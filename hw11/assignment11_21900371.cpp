#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	//background subtraction
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	VideoCapture capture("background.mp4");
	Mat image, mask, background;

	//foreground
	VideoCapture cap;
	Mat origin;
	Mat frame;
	Mat foregroundImg;
	Mat element1, element2,element3;

	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;
	
	
	if (cap.open("background.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}
	
	while (1) {
		//Read 'background.mp4' as gray-scale
		cap >> origin;
		if (origin.empty()) {
			break;
		}

		cvtColor(origin, frame, CV_BGR2GRAY);

		image = frame.clone();
		image.copyTo(foregroundImg);

		mask.create(image.size(), image.type());
		bg_model->apply(image, mask);
		bg_model->getBackgroundImage(background);

		//subtranction
		absdiff(frame, background, foregroundImg);
		threshold(foregroundImg, foregroundImg, 50, 255, CV_THRESH_BINARY);
		element1 = getStructuringElement(MORPH_RECT, Size(11, 11));
		element2 = getStructuringElement(MORPH_ELLIPSE, Size(13,13));
		element3 = getStructuringElement(MORPH_RECT, Size(5, 5));
		//erode(foregroundImg, foregroundImg, element2);
		//dilate(foregroundImg, foregroundImg, element1);
		//dilate(foregroundImg, foregroundImg, element1);
		dilate(foregroundImg, foregroundImg, element1);
		morphologyEx(foregroundImg, foregroundImg, MORPH_CLOSE, element2);
		erode(foregroundImg, foregroundImg, element3);
		findContours(foregroundImg, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		int count1 = 0;

		vector<Rect> boundRect(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			boundRect[i] = boundingRect(Mat(contours[i]));
		}
		for (int i = 0; i < contours.size(); i++) {
			if (boundRect[i].area() > 600) {
				count1++;
				
				rectangle(origin, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		putText(origin, format("Number of people:%d", count1), Point(50, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(128), 4);
		
		
		imshow("fore", foregroundImg);
		imshow("Number of people", origin);


		double fps = cap.get(CAP_PROP_FPS);
		waitKey(1000 / fps);
	}
	
	return 0;
}
