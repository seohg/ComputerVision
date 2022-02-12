#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture capture("background.mp4");
	Mat image, sum, avg, background;
	int cnt = 2;

	Mat frame;
	VideoCapture cap;
	Mat foregroundImg;

	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;

	capture >> avg;
	cvtColor(avg, avg, CV_BGR2GRAY);

	while (cnt < 11) {
		if (!capture.read(image)) break;
		cvtColor(image, image, CV_BGR2GRAY);
		add(image / cnt, (avg * (cnt - 1)) / cnt, avg);

		cnt++;
	}
	background = avg;
	
	if (cap.open("background.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}


	while (1) {
		cap >> frame;
		if (frame.empty()) {
			break;
		}
		cvtColor(frame, frame, CV_BGR2GRAY);
		absdiff(frame, background, foregroundImg);
		threshold(foregroundImg, foregroundImg, 20, 255, CV_THRESH_BINARY);
		findContours(foregroundImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		int count1 = 0;

		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			boundRect[i] = boundingRect(Mat(contours[i]));
		}
		for (int i = 0; i < contours.size(); i++) {
			if (boundRect[i].area() > 400) {
				count1++;
				rectangle(frame, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		putText(frame, format("count:%d", count1), Point(50, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(128), 4);
		
		
		
		imshow("background subtraction", frame);


		double fps = cap.get(CAP_PROP_FPS);
		waitKey(1000 / fps);
	}

	return 0;
}
