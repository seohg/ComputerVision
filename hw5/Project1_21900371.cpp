#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main() {
	int key;
	//LENA n : negative transformation, g : gamma transformation , h : histogram equalization, r : Reset 
	Mat lena = imread("lena.png", 1);
	Mat lena_t;
	Mat lena_h[3];
	Mat lena_gamma;

	Mat colorful = imread("colorful.jpg", 1);
	Mat colorful_t;
	Mat colorful_hsv[3];

	Mat balancing = imread("balancing.jpg", 1);
	Mat balancing_t;
	Mat balancing_bgr[3];

	if (!lena.data || !colorful.data || !balancing.data) exit(1);
	lena_t = lena.clone();
	colorful_t = colorful.clone();
	balancing_t = balancing.clone();

	imshow("lena", lena_t);
	imshow("colorful", colorful_t);
	imshow("balancing", balancing_t);

	while (1) {
		key = waitKey();
		if (key == 27) { // ESC
			break;
		}
		else if (key == 114) { // Reset : 'r'
			lena_t = lena.clone();
			colorful_t = colorful.clone();
			balancing_t = balancing.clone();
			imshow("lena", lena_t);
			imshow("colorful", colorful_t);
			imshow("balancing", balancing_t);
		}
		else if (key == 110) { // negative transformation : 'n'
			cvtColor(lena_t, lena_t, CV_BGR2HSV);
			split(lena_t, lena_h);
			for (int i = 0; i < lena_t.rows; i++) {
				for (int j = 0; j < lena_t.cols; j++) {
					lena_h[2].at<uchar>(i, j) = 255-lena_h[2].at<uchar>(i, j);
				}
			}
			merge(lena_h, 3, lena_t);
			cvtColor(lena_t, lena_t, CV_HSV2BGR);
			imshow("lena", lena_t);
		}
		else if (key == 103) { // gamma transformation : 'g'
			cvtColor(lena_t, lena_t, CV_BGR2HSV);
			split(lena_t, lena_h);
			lena_h[2].convertTo(lena_gamma, CV_32F);
			lena_gamma = lena_gamma / 255.0;
			for (int i = 0; i < lena_gamma.rows; i++) {
				for (int j = 0; j < lena_gamma.cols ; j++) {
					lena_gamma.at<float>(i, j) = pow(lena_gamma.at<float>(i, j), 2.5f) * 255.0f;
				}
			}
			lena_gamma.convertTo(lena_h[2], CV_8U);
			merge(lena_h, 3, lena_t);
			cvtColor(lena_t, lena_t, CV_HSV2BGR);
			imshow("lena", lena_t);
		}
		else if (key == 104) { // histogram equalization : 'h'
			cvtColor(lena_t, lena_t, CV_BGR2HSV);
			split(lena_t, lena_h);
			equalizeHist(lena_h[2], lena_h[2]);
			merge(lena_h, 3, lena_t);
			cvtColor(lena_t, lena_t, CV_HSV2BGR);
			imshow("lena", lena_t);
		}
		else if (key == 115) { //color slicing : 's'
			cvtColor(colorful_t, colorful_t, CV_BGR2HSV);
			split(colorful_t, colorful_hsv);
			for (int i = 0; i < colorful_t.rows; i++) {
				for (int j = 0; j < colorful_t.cols; j++) {
					if (colorful_hsv[0].at<uchar>(i, j) > 9 && colorful_hsv[0].at<uchar>(i, j) < 23) {
						colorful_hsv[1].at<uchar>(i, j) = colorful_hsv[1].at<uchar>(i, j);
					}
					else {
						colorful_hsv[1].at<uchar>(i, j) = 0;
					}
				}
			}
			merge(colorful_hsv, 3, colorful_t);
			cvtColor(colorful_t, colorful_t, CV_HSV2BGR);
			imshow("colorful", colorful_t);
		}
		else if (key == 99) { //color conversion : 'c'
			cvtColor(colorful_t, colorful_t, CV_BGR2HSV);
			split(colorful_t, colorful_hsv);
			for (int i = 0; i < colorful_t.rows; i++) {
				for (int j = 0; j < colorful_t.cols; j++) {
					int tmp = colorful_hsv[0].at<uchar>(i, j);
					if (tmp > 129) {
						colorful_hsv[0].at<uchar>(i, j) -= 129;
					}
					else {
						colorful_hsv[0].at<uchar>(i, j) += 50;
					}
				}
			}
			merge(colorful_hsv, 3, colorful_t);
			cvtColor(colorful_t, colorful_t, CV_HSV2BGR);
			imshow("colorful", colorful_t);
		}
		else if (key == 97) { //average filtering : 'a'
			cvtColor(balancing_t, balancing_t, CV_BGR2HSV);
			split(balancing_t, balancing_bgr);
			blur(balancing_bgr[2], balancing_bgr[2], Size(9, 9));
			merge(balancing_bgr, 3, balancing_t);
			cvtColor(balancing_t, balancing_t, CV_HSV2BGR);
			imshow("balancing", balancing_t);
		}
		else if (key == 119) {
			split(balancing_t, balancing_bgr);
			int B = 0, G = 0, R = 0;
			int B1, G1, R1;
			for (int i = 0; i < balancing_t.rows; i++) {
				for (int j = 0; j < balancing_t.cols; j++) {
					B += balancing_bgr[0].at<uchar>(i, j);
					G += balancing_bgr[1].at<uchar>(i, j);
					R += balancing_bgr[2].at<uchar>(i, j);
				}
			}
			B = B / (balancing_t.rows * balancing_t.cols);
			G = G / (balancing_t.rows * balancing_t.cols);
			R = R / (balancing_t.rows * balancing_t.cols);

			for (int i = 0; i < balancing_t.rows; i++) {
				for (int j = 0; j < balancing_t.cols; j++) {
					B1 = balancing_bgr[0].at<uchar>(i, j);
					G1 = balancing_bgr[1].at<uchar>(i, j);
					R1 = balancing_bgr[2].at<uchar>(i, j);

					B1 = 128 * balancing_bgr[0].at<uchar>(i, j)/B;
					if (B1> 255) {
						balancing_bgr[0].at<uchar>(i, j) = 255;
					}else{
						balancing_bgr[0].at<uchar>(i, j) = B1;
					}

					G1 = 128 * balancing_bgr[1].at<uchar>(i, j)/G;

					if (G1 > 255) {
						balancing_bgr[1].at<uchar>(i, j) = 255;
					}
					else {
						balancing_bgr[1].at<uchar>(i, j) = G1;
					}

					R1 =128 * balancing_bgr[2].at<uchar>(i, j)/R;
					if (R1 > 255) {
						balancing_bgr[2].at<uchar>(i, j) = 255;
					}
					else {
						balancing_bgr[2].at<uchar>(i, j) = R1;
					}
				}
			}
			merge(balancing_bgr,3, balancing_t);
			imshow("balancing", balancing_t);
		}
	}
	return 0;
}
