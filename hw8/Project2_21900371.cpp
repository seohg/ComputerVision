#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame;
	VideoCapture cap;
	Mat left, center, right;

	Mat image, edge, result;
	float rho1, theta1, a1, b1, x01, y01;
	float rho2, theta2, a2, b2, x02, y02;
	float rho1_sum, theta1_sum, count1;
	float rho2_sum, theta2_sum, count2;
	bool i1 = false, i2 = false;
	Point p11, p21;
	Point p12, p22;
	vector<Vec2f> lines1;
	vector<Vec2f> lines2;

	Rect left_roi(230, 400, 390, 200);
	Rect center_roi(620, 400, 60, 200);
	Rect right_roi(680, 400, 395, 200);

	if (cap.open("Road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	while (1) {
		cap >> frame;
		if (frame.empty()) {
			cout << "end of video" << endl;
			break;
		}

		double sec = cap.get(CAP_PROP_POS_MSEC);
		if (sec > 35000) {
			break;
		}


		image = frame.clone();
		result = frame.clone();

		cvtColor(image, image, CV_BGR2GRAY);
		blur(image, image, Size(5, 5));
		Canny(image, edge, 20, 200, 3);

		left = edge(left_roi);
		center = edge(center_roi);
		right = edge(right_roi);

		HoughLines(center, lines1, 1, CV_PI / 180, 60);
		HoughLines(center, lines2, 1, CV_PI / 180, 60);


		i1 = false;
		rho1_sum = 0;
		theta1_sum = 0;
		count1 = 0;

		for (int i = 0; i < lines1.size(); i++) {
			rho1 = lines1[i][0];
			theta1 = lines1[i][1];
			if (theta1 * 180.0 / CV_PI >= 10 && theta1 * 180.0 / CV_PI <= 60) {
				i1 = true;
				rho1_sum += rho1;
				theta1_sum += theta1;
				count1++;
			}
		}


		i2 = false;
		rho2_sum = 0;
		theta2_sum = 0;
		count2 = 0;

		for (int i = 0; i < lines2.size(); i++) {
			rho2 = lines2[i][0];
			theta2 = lines2[i][1];
			if (theta2 * 180.0 / CV_PI >= 150 && theta2 * 180.0 / CV_PI <= 170) {
				i2 = true;
				rho2_sum += rho2;
				theta2_sum += theta2;
				count2++;
			}

		}

		if (i1 == true) {
			rho1_sum /= count1;
			theta1_sum /= count1;
			a1 = cos(theta1_sum);
			b1 = sin(theta1_sum);
			x01 = a1 * rho1_sum;
			y01 = b1 * rho1_sum;
			p11 = Point(cvRound(x01 + (image.rows + image.cols) * (-b1)) + 630, cvRound(y01 + (image.rows + image.cols) * (a1)) + 400);
			p21 = Point(cvRound(x01 - (image.rows + image.cols) * (-b1)) + 630, cvRound(y01 - (image.rows + image.cols) * (a1)) + 400);
			//line(result, p11, p21, Scalar(0, 0, 255), 3, 8);
			putText(result, format("Warning!: Lane departure"), Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 4);
			//printf("frame1");

		}

		if (i2 == true) {
			rho2_sum /= count2;
			theta2_sum /= count2;
			a2 = cos(theta2_sum);
			b2 = sin(theta2_sum);
			x02 = a2 * rho2_sum;
			y02 = b2 * rho2_sum;
			p12 = Point(cvRound(x02 + (image.rows + image.cols) * (-b2)) + 630, cvRound(y02 + (image.rows + image.cols) * (a2)) + 400);
			p22 = Point(cvRound(x02 - (image.rows + image.cols) * (-b2)) + 630, cvRound(y02 - (image.rows + image.cols) * (a2)) + 400);
			//line(result, p12, p22, Scalar(0, 0, 255), 3, 8);
			putText(result, format("Warning!: Lane departure"), Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(0, 0, 255), 4);
			//printf("frame2");
		}

		imshow("Frame", result);

		waitKey(1000 / cap.get(CAP_PROP_FPS));
	}
	return 0;
}

