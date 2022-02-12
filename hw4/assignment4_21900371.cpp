#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

Mat drqwHistogram(Mat src) {
	Mat hist, histImage;

	int i, hist_w, hist_h, bin_w, histSize;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	hist_w = 512;
	hist_h = 512;
	histSize = 16;
	bin_w = cvRound((double)hist_w / histSize);

	//draw the histogram
	histImage = Mat(hist_h, hist_w, CV_8UC3, Scalar(255, 255, 255));
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);
	normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

	for (i = 0; i < histSize; i++) {
		rectangle(histImage, Point(bin_w * i, hist_h), Point(bin_w * i + hist_w / histSize, hist_h - cvRound(hist.at<float>(i))), Scalar(0, 0, 0), -1);
	}
	return histImage;
}

void computeHistogram(Mat src, Mat img) {
	Mat hist;

	int i, hist_w, hist_h, bin_w, histSize;
	float range[] = { 0, 256 };
	const float* histRange = { range };

	hist_w = src.cols;
	hist_h = src.rows;
	histSize = 8;

	//draw the histogram
	calcHist(&src, 1, 0, Mat(), hist, 1, &histSize, &histRange);

	//print text
	for (i = 0; i < histSize; i++) {
		putText(img, format("%d bin : %f ", i + 1, hist.at<float>(i) = hist.at<float>(i) / (hist_w * hist_h)), Point(10, i * 30 + 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(255, 0, 0), 1);
	}
}


int main() {
	Mat moon = imread("moon.png", 0);
	Mat before = moon.clone();
	Mat after;
	Mat beforeC, afterC;
	Mat before_graph;
	Mat after_graph;

	if (!moon.data) exit(1);

	//histogram equalization 
	equalizeHist(before, after);
	beforeC = before.clone();
	afterC = after.clone();

	//compute normailized histogram	
	computeHistogram(beforeC, before);
	computeHistogram(afterC, after);

	imshow("before", before);
	imshow("after", after);

	//Display histogram;
	before_graph = drqwHistogram(before);
	after_graph = drqwHistogram(after);

	imshow("h1", before_graph);
	imshow("h2", after_graph);

	waitKey(0);
}
