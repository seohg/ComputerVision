#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	//background subtraction
	Ptr<BackgroundSubtractor> bg_model = createBackgroundSubtractorMOG2();
	VideoCapture capture("background.mp4");
	Mat image, mask, background;
	Rect rect1(0,0,720,135);
	Rect rect2(0,135, 720, 160);
	Rect rect3(0, 295, 720, 281);
	int count;
	Mat roi1, roi2, roi3;


	//foreground
	VideoCapture cap;
	Mat origin;
	Mat frame;
	Mat foregroundImg;
	Mat element1, element2, element3, element4,element5;

	vector<vector<Point>> contours1;
	vector<vector<Point>> contours2;
	vector<vector<Point>> contours3;
	vector<Vec4i>hierarchy1;
	vector<Vec4i>hierarchy2;
	vector<Vec4i>hierarchy3;

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
		count = 0;
		absdiff(frame, background, foregroundImg);
		element1 = getStructuringElement(MORPH_RECT, Size(11, 11));
		element2 = getStructuringElement(MORPH_ELLIPSE, Size(13, 13));
		element3 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
		element4 = getStructuringElement(MORPH_ELLIPSE, Size(9, 9));
		element5 = getStructuringElement(MORPH_ELLIPSE, Size(7, 7));
		//roi1; 
		roi1 = foregroundImg(rect1);
		threshold(roi1, roi1, 60, 255, CV_THRESH_BINARY);
		dilate(roi1, roi1, element1);
		morphologyEx(roi1, roi1, MORPH_CLOSE, element2);
		erode(roi1, roi1, element4);

		//roi2; 
		roi2 = foregroundImg(rect2);
		threshold(roi2, roi2, 20, 255, CV_THRESH_BINARY);
		//erode(roi2, roi2, element5);
		morphologyEx(roi2, roi2, MORPH_CLOSE, element2);
		//dilate(roi2, roi2, element5);

		//roi3; 
		roi3 = foregroundImg(rect3);
		threshold(roi3, roi3, 30, 255, CV_THRESH_BINARY);
		dilate(roi3, roi3, element3);
		morphologyEx(roi3, roi3, MORPH_OPEN, element4);
		//dilate(roi3, roi3, element1);
		erode(roi3, roi3, element4);


		findContours(roi1, contours1, hierarchy1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect1(contours1.size());
		for (int i = 0; i < contours1.size(); i++) {
			boundRect1[i] = boundingRect(Mat(contours1[i]));
		}
		for (int i = 0; i < contours1.size(); i++) {
			if (boundRect1[i].area() > 700) {
				count++;
				printf("x : %d y : %d\n", boundRect1[i].x, boundRect1[i].y);
				printf("area : %d\n ", boundRect1[i].area());
				rectangle(origin, boundRect1[i].tl(), boundRect1[i].br(), Scalar(0, 0, 255), 2, 8, 0);
			}
		}

		findContours(roi2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect2(contours2.size());
		for (int i = 0; i < contours2.size(); i++) {
			boundRect2[i] = boundingRect(Mat(contours2[i]));
		}
		for (int i = 0; i < contours2.size(); i++) {
			if (boundRect2[i].area() > 4500) {
				count++;
				printf("x : %d y : %d\n", boundRect2[i].x, boundRect2[i].y);
				printf("area : %d\n ", boundRect2[i].area());
				int x = boundRect2[i].tl().x;
				int y = boundRect2[i].tl().y + 135;
				int bx = boundRect2[i].br().x;
				int by = boundRect2[i].br().y + 135;
				rectangle(origin, Point(x, y), Point(bx, by), Scalar(0, 0, 255), 2, 8, 0);
			}
		}

		findContours(roi3, contours3, hierarchy3, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect3(contours3.size());
		for (int i = 0; i < contours3.size(); i++) {
			boundRect3[i] = boundingRect(Mat(contours3[i]));
		}
		for (int i = 0; i < contours3.size(); i++) {
			if (boundRect3[i].area() > 4000) {
				if (abs(boundRect3[i].tl().x - boundRect3[i].br().x) > abs(boundRect3[i].tl().y - boundRect3[i].br().y)) {
					if (boundRect3[i].area() > 35000) {
						count++;
					}
				}
				else {
					if (boundRect3[i].area() > 25000) {
						count++;
					}
				}
				
				count++;
				printf("x : %d y : %d\n", boundRect3[i].x, boundRect3[i].y);
				printf("area : %d\n ", boundRect3[i].area());
				int x = boundRect3[i].tl().x;
				int y = boundRect3[i].tl().y + 295;
				int bx = boundRect3[i].br().x;
				int by = boundRect3[i].br().y + 295;
				rectangle(origin, Point(x, y), Point(bx, by), Scalar(0, 0, 255), 2, 8, 0);
			}
		}
		//erode(foregroundImg, foregroundImg, element2);
		//dilate(foregroundImg, foregroundImg, element1);
		//dilate(foregroundImg, foregroundImg, element1);
		//dilate(roi1, roi1, element1);
		//morphologyEx(roi1, roi1, MORPH_CLOSE, element2);
		//erode(roi1, roi1, element3);
		/*
		//roi2; 
		roi2 = foregroundImg(rect2);
		threshold(roi2, roi2, 60, 255, CV_THRESH_BINARY);
		dilate(roi2, roi2, element1);
		morphologyEx(roi2, roi2, MORPH_CLOSE, element2);
		erode(roi2, roi2, element4);
		findContours(roi2, contours2, hierarchy2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

		vector<Rect> boundRect2(contours2.size());
		for (int i = 0; i < contours2.size(); i++) {
			boundRect2[i] = boundingRect(Mat(contours2[i]));
		}
		for (int i = 0; i < contours2.size(); i++) {
			if (boundRect2[i].area() > 800) {
				count++;
				printf("x : %d y : %d\n", boundRect2[i].x, boundRect2[i].y);
				printf("area : %d\n ", boundRect2[i].area());
				int x = boundRect2[i].tl().x;
				int y = boundRect2[i].tl().y + 135;
				int bx = boundRect2[i].br().x;
				int by = boundRect2[i].br().y + 135;
				rectangle(origin, Point(x,y), Point(bx, by), Scalar(0, 0, 255), 2, 8, 0);
			}
		}

		*/

		putText(origin, format("Number of people:%d", count), Point(50, 80), FONT_HERSHEY_SIMPLEX, 1, Scalar(128), 4);


		//imshow("fore", foregroundImg);
		imshow("Number of people", origin);


		double fps = cap.get(CAP_PROP_FPS);
		waitKey(1000 / fps);
	}

	return 0;
}
