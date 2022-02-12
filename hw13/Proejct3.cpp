#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;



int main() {
	// cam
	VideoCapture cap(0);
	Mat frame;

	//for detection
	CascadeClassifier face_classifier;
	vector<Rect> faces;
	Mat  grayframe;
	Rect face_rc;
	

	bool detection = true;
	bool rc = false;
	
	//for tracking
	Mat m_backproj, hsv;
	Mat m_model3d;
	Rect m_rc;

	float hrange[] = { 0,180 };
	float vrange[] = { 0,255 };
	float srange[] = { 0,255 };
	const float* ranges[] = { hrange, srange, vrange };
	int channels[] = { 0,1,2 };
	int hist_sizes[] = { 16,16,16 };

	bool tracking = false;

	//for changing background
	Mat result, bgModel, fgModel, image, foreground;
	Mat resultImage;


	//background image
	image = imread("backImg.jpg");
	resize(image, image, image.size()/8, CV_INTER_AREA);

	//face detection
	face_classifier.load("C:/Users/shk98/Downloads/opencv/sources/data/haarcascades/haarcascade_frontalface_alt.xml");

	//check if we succeded
	if (!cap.isOpened()) {
		cout << "can't open video file" << endl;
		return 0;
	}
	int count = 0;
	while (true) {
		// load frame
		cap >> frame;
		resize(frame, frame, image.size(), CV_INTER_AREA);
		//imshow("Tracker", frame);

		if (frame.empty()) break;
		if (waitKey(33) == 27) break;


		//face detection
		cvtColor(frame, grayframe, COLOR_BGR2GRAY);
		//face tracking
		cvtColor(frame, hsv, COLOR_BGR2HSV);

		//face detection
		if (detection) {
			face_classifier.detectMultiScale(
				grayframe,
				faces,
				1.1, //increase search scale by 10% each pass
				3, //merge groups of three detections
				0, //not used for a new cascade
				Size(10, 10) //minsize
			);

			if (faces.size()) {
				face_rc = faces[0];
				rc = true;
				Mat mask = Mat::zeros(face_rc.height, face_rc.width, CV_8U);
				ellipse(mask, Point(face_rc.width / 2, face_rc.height / 2), Size(face_rc.width / 2, face_rc.height / 2), 0, 0, 360, 255, CV_FILLED);
				Mat roi(hsv, face_rc);
				calcHist(&roi, 1, channels, mask, m_model3d, 3, hist_sizes, ranges);
				tracking = true;
				detection = false;
				//rectangle(frame, face_rc, Scalar(0, 255, 0), 3);
				//imshow("detection", frame);
				printf("detection");
			}
			else {
				tracking = false;
				detection = true;
			}
		}
		
		//face tracking
		if (tracking) {
			printf("tracking\n");
			m_rc = face_rc;
			//histogram backprojection
			calcBackProject(&hsv, 1, channels, m_model3d, m_backproj, ranges);
			//tracking
			CamShift(m_backproj, m_rc, TermCriteria(TermCriteria::EPS | TermCriteria::COUNT, 20, 1));
			printf("%d %d\n", m_rc.height, m_rc.width);
			rectangle(frame, m_rc, Scalar(0, 0, 255), 3);
			//imshow("detectionnew", frame);
			
		}

		if (rc) {
			if (tracking) {
				grabCut(frame, result, m_rc, bgModel, fgModel, 6, GC_INIT_WITH_RECT);
				compare(result, GC_PR_FGD, result, CMP_EQ);
				resultImage = image.clone();
				frame.copyTo(resultImage, result);
			}else if (detection) {
				grabCut(frame, result, face_rc, bgModel, fgModel, 6, GC_INIT_WITH_RECT);
				compare(result, GC_PR_FGD, result, CMP_EQ);
				resultImage = image.clone();
				frame.copyTo(resultImage, result);
			}
			
		}
		else {
			resultImage = image.clone();
		}
		count++;
		if (count % 10 == 1 || count % 10 == 4 || count % 10 == 8) {
			tracking = false;
			detection = true;
		}
		
		//imshow("result", frame);
		imshow("Project3", resultImage);
	}
	

	return 0;
}