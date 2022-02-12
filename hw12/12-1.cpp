#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	CascadeClassifier face_classifier;
	vector<Rect> faces;

	Mat frame, grayframe;
	Rect max;
	Rect min;
	Rect middle;
	int key = 0;
	int count = 0;


	//open video
	VideoCapture cap("Faces.mp4");
	double fps = cap.get(CV_CAP_PROP_FPS);

	//check video is open
	if (!cap.isOpened()) {
		cout << "Could not open video" << endl;
		return -1;
	}

	//face detection configuration
	face_classifier.load("C:/Users/shk98/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml");

	while (true) {
		//get a new frame from video
		cap >> frame;
		if (frame.empty()) {
			break;
		}
		//resize(frame, frame, Size(360, 288), 0, 0, CV_INTER_AREA);

		//convert captured frame to grayscale
		cvtColor(frame, grayframe, COLOR_BGR2GRAY);

		face_classifier.detectMultiScale(
			grayframe,
			faces,
			1.1, //increase search scale by 10% each pass
			3, //merge groups of three detections
			0, //not used for a new cascade
			Size(30, 30), //minsize
			Size(100, 100)
		);


		int newkey = waitKey(1000 / fps);
		if (newkey == -1) {
			key = key;
		}
		else {
			key = newkey;
		}
		//printf("%d", key);
		//key = waitKey(1000 / fps);
		if (key == 110) { // n : detect the nearest face
			//draw result
			max = faces[0];
			for (int i = 0; i < faces.size(); i++) {
				if (faces[i].area() > max.area() && (faces[i].area() < 10000)) {
					max = faces[i];
					printf("%d\n", max.area());
				}
			}
			Point lb(max.x + max.width, max.y + max.height);
			Point tr(max.x, max.y);
			rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
		}
		else if (key == 102) { // f : detect the farthest face
		   //draw result
			min = faces[0];
			for (int i = 0; i < faces.size(); i++) {
				if (faces[i].area() < min.area()) {
					min = faces[i];
				}
			}
			Point lb(min.x + min.width, min.y + min.height);
			Point tr(min.x, min.y);

			rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
		}
		else if (key == 109) { // m : detect the fact at the middle range
			count = 0;
			min = faces[0]; max = faces[0]; middle = faces[0];

			for (int i = 0; i < faces.size(); i++) {
				if (faces[i].area() > max.area() && (faces[i].area() < 10000)) {
					max = faces[i];
				}
				if (faces[i].area() < min.area()) {
					min = faces[i];
				}
			}
			middle = min;
			for (int i = 0; i < faces.size(); i++) {
				if (faces[i].area() > min.area() && faces[i].area() < max.area() && (faces[i].area() < 10000)) {
					middle = faces[i];
				}
			}




			Point lb(middle.x + middle.width, middle.y + middle.height);
			Point tr(middle.x, middle.y);

			rectangle(frame, lb, tr, Scalar(0, 255, 0), 3, 4, 0);
		}

		//print the output
		imshow("Faces", frame);
	}


	return 0;
}