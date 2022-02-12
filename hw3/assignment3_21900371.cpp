#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat lena = imread("lena.png", 0);
	Mat moon = imread("moon.png", 0);
	Mat saltnpepper = imread("saltnpepper.png", 0);
	Mat in_blur = lena.clone();
	Mat sharp = moon.clone();
	Mat median = saltnpepper.clone();
	Mat lena_out = lena.clone();
	Mat moon_out, saltnpepper_out;

	Rect rect1(0, 0, in_blur.size().width / 2, in_blur.size().height);

	// Average filtering
	in_blur = lena_out(rect1);
	blur(in_blur, in_blur, Size(7, 7));
	lena_out(rect1) = in_blur;

	imshow("lena", lena);
	imshow("lena_filtered", lena_out);

	//sharpening
	moon_out = Mat::zeros(moon.size(), moon.type());

	for (int i = 0; i < moon.size().width / 2; i++) {
		for (int j = 0; j < moon.size().height; j++) {
			moon_out.at<uchar>(j, i) = moon.at<uchar>(j, i);
		}
	}

	Laplacian(moon, sharp, CV_16S);
	convertScaleAbs(sharp, sharp);
	add(moon, sharp, sharp);

	for (int i = moon.size().width / 2; i < moon.size().width; i++) {
		for (int j = 0; j < moon.size().height; j++) {
			moon_out.at<uchar>(j, i) = sharp.at<uchar>(j, i);
		}
	}
	imshow("moon", moon);
	imshow("moon_filtered", moon_out);


	//median
	medianBlur(saltnpepper, saltnpepper_out, 9);

	imshow("saltnpepper", saltnpepper);
	imshow("saltnpepper_filtered", saltnpepper_out);
	
	waitKey(0);


}
