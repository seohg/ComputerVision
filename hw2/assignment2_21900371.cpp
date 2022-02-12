#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat image = imread("lena.png", 0);
	Mat negative_img;
	Mat log_img, log_img_float;
	Mat gamma_img, gamma_img_float;

	int channels;

	channels = image.channels();
	imshow("origin", image);

	//negative transformation
	negative_img = image.clone();

	for (int i = 0; i < negative_img.cols; i++) {
		for (int j = 0; j < negative_img.rows; j++) {
			negative_img.at<uchar>(j, i) = 255 - negative_img.at<uchar>(j, i);
		}
	}
	imshow("negative", negative_img);

	//log transformation
	log_img = image.clone();
	log_img.convertTo(log_img_float, CV_32F);
	log_img_float = log_img_float + 1;
	log(log_img_float, log_img_float);
	normalize(log_img_float, log_img_float, 0, 255, NORM_MINMAX);
	log_img_float = 1.5 * log_img_float;
	convertScaleAbs(log_img_float, log_img);
	imshow("log", log_img);

	//Gamma transformation(g = 0.5)
	gamma_img = image.clone();
	gamma_img.convertTo(gamma_img_float, CV_32F);
	//normalize(log_img_float, log_img_float, 0, 255, NORM_MINMAX);
	gamma_img_float = gamma_img_float / 255.0;
	for (int i = 0; i < gamma_img_float.cols; i++) {
		for (int j = 0; j < gamma_img_float.rows; j++) {
			gamma_img_float.at<float>(j, i) = pow(gamma_img_float.at<float>(j, i), 0.5f) * 255.0f;
		}
	}
	gamma_img_float.convertTo(gamma_img, CV_8U);
	imshow("gamma", gamma_img);
	waitKey(0);
}