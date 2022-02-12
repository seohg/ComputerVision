#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	VideoCapture cap;

	Mat canny;

	Mat canny_left;
	Mat canny_right;

	Rect left_roi(200, 400, 400, 200);
	Rect right_roi(600, 400, 400, 200);

	if (cap.open("Road.mp4") == 0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}

	while (1) {
		cap >> canny;
		if (canny.empty()) {
			cout << "end of video" << endl;
			break;
		}

		cvtColor(canny, canny, CV_BGR2GRAY);
		blur(canny, canny, Size(5, 5));
		Canny(canny, canny, 10, 60, 3);

		canny_left = canny(left_roi);
		canny_right = canny(right_roi);


		namedWindow("Left canny");
		moveWindow("Left canny", 200, 0);
		imshow("Left canny", canny_left);
		namedWindow("Right canny");
		moveWindow("Right canny", 600, 0);
		imshow("Right canny", canny_right);

		
		waitKey(1000 / cap.get(CAP_PROP_FPS));
	}
	return 0;
}
