#include "cv.hpp"
#include <iostream>
#include <opencv2/dnn.hpp>
#include <fstream>

using namespace cv;
using namespace std;
using namespace dnn;

int main() {
	//point vector
	vector<Point2f> in, out;

	//for namecard point
	vector<Point> card;
	Rect cardRect;

	//for line
	float rho1, theta1, a1, b1, x01, y01;
	float rho1_sum, theta1_sum, count1;
	Point p11, p21;
	vector<Vec2f> lines1;

	//image
	Mat imput = imread("namecard.jpg");
	Mat gray = imread("namecard.jpg");
	Mat edge;
	Mat result;

	//resize
	if (gray.channels() == 4) cvtColor(gray, gray, COLOR_BGRA2BGR);
	resize(imput, imput, Size(460, 460), CV_INTER_AREA);
	resize(gray, gray, Size(460, 460),CV_INTER_AREA);
	
	//for detection
	//blur
	GaussianBlur(gray, gray, Size(7, 7), 5, 5, BORDER_DEFAULT);

	//edge(line) detection
	Canny(gray, edge, 50, 200, 3);
	threshold(edge, result, 100, 255, THRESH_BINARY_INV);
	HoughLines(edge, lines1, 1, CV_PI / 180, 105);
	
	//draw line
	for (int i = 0; i < lines1.size(); i++) {
		rho1 = lines1[i][0];
		theta1 = lines1[i][1];	
		a1 = cos(theta1);
		b1 = sin(theta1);
		x01 = a1 * rho1;
		y01 = b1 * rho1;
		p11 = Point(cvRound(x01 + (result.rows + result.cols) * (-b1)) , cvRound(y01 + (result.rows + result.cols) * (a1)));
		p21 = Point(cvRound(x01 - (result.rows + result.cols) * (-b1)) , cvRound(y01 - (result.rows + result.cols) * (a1)));
		line(result, p11, p21, Scalar(0, 0, 255), 3, 8);
	}
	
	//find namecard
	vector<vector<Point>> contours;
	vector<Vec4i>hierarchy;

	findContours(result, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	
	int max = 0; 
	for (int i = 0; i < contours.size(); i++) {
		Rect BoundRect = boundingRect(Mat(contours[i]));
		if (BoundRect.area() > max) {
			max = BoundRect.area();
			cardRect = BoundRect;
			card = contours[i];
		}	
	}
	
	//find namecard point
	float maxN = gray.rows*gray.rows +gray.cols*gray.cols;
	float minN = 0;
	//first point
	int fi=0;
	for (int i = 0; i < card.size(); i++) {
		float dist = sqrt(card[i].x* card[i].x + card[i].y*card[i].y);
		if (dist < maxN) {
			maxN = dist;
			fi = i;
		}
	}
	//third point
	int ti = 0;
	for (int i = 0; i < card.size(); i++) {
		float dist = abs(card[i].x-card[fi].x) * abs(card[i].x - card[fi].x)
			+ abs(card[i].y - card[fi].y) * abs(card[i].y - card[fi].y);
		
		if (dist >minN) {
			minN = dist;
			ti = i;
		}
	}
	//4th point
	int f4 = 0;
	minN = 0;
	for (int i = 0; i < card.size(); i++) {
		float dist1 = sqrt((card[i].x - card[fi].x) * (card[i].x - card[fi].x)
			+ (card[i].y - card[fi].y) * (card[i].y - card[fi].y));
		float dist2 = sqrt((card[i].x - card[ti].x) * (card[i].x - card[ti].x)
			+ (card[i].y - card[ti].y) * (card[i].y - card[ti].y));
		float dist = dist1 + dist2;
		if (dist > minN && card[i].y>card[fi].y&&card[i].x<card[ti].x) {
			minN = dist;
			f4 = i;
		}
	}
	//second point
	int si=0;
	minN = 0;
	for (int i = 0; i < card.size(); i++) {
		float dist1 = sqrt((card[i].x - card[fi].x) * (card[i].x - card[fi].x)
			+ (card[i].y - card[fi].y) * (card[i].y - card[fi].y));
		float dist2 = sqrt((card[i].x - card[ti].x) * (card[i].x - card[ti].x)
			+ (card[i].y - card[ti].y) * (card[i].y - card[ti].y));
		float dist = dist1 + dist2;
		if (dist > minN && card[i].x > card[fi].x && card[i].y < card[ti].y) {
			minN = dist;
			si = i;
		}
	}
	//save point
	int f1x = card[fi].x;
	int f1y = card[fi].y;
	int sx = card[si].x;
	int sy = card[si].y;
	int tx = card[ti].x;
	int ty = card[ti].y;
	int f4x = card[f4].x;
	int f4y = card[f4].y;
	
	//push in vector
	in.push_back(Point2f(card[fi].x, card[fi].y));
	in.push_back(Point2f(card[si].x, card[si].y));
	in.push_back(Point2f(card[ti].x, card[ti].y));
	in.push_back(Point2f(card[f4].x, card[f4].y));
	
	//push out vector
	out.push_back(Point2f(0, 0));
	out.push_back(Point2f(416, 0));
	out.push_back(Point2f(416, 416));
	out.push_back(Point2f(0, 416));
	
	//ComputeHomography from 4 matching pairs
	Mat homo_mat = getPerspectiveTransform(in, out);

	//Image warping
	warpPerspective(imput, result, homo_mat, Size(416, 416));

	//print image
	imshow("namecard", result);
	waitKey(0);
	return 0;
}