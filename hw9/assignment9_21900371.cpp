#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat finge_print = imread("Finger.png", 0);
	Mat adaptive_1 = imread("adaptive_1.jpg", 0);
	Mat adaptive = imread("adaptive.png", 0);

	threshold(finge_print, finge_print, 0, 255, THRESH_BINARY|THRESH_OTSU);
	
	adaptiveThreshold(adaptive_1, adaptive_1, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 15);

	adaptiveThreshold(adaptive, adaptive, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 9, 15);



	imshow("finge_print", finge_print);
	imshow("adaptive_1", adaptive_1);
	imshow("adaptive", adaptive);

	waitKey(0);

	return 0;
}
