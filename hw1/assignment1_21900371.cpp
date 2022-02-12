#include "cv.hpp"
#include <iostream>

using namespace std;
using namespace cv;

int main() {
	Mat frame;
	VideoCapture cap;
	
	if (cap.open("background.mp4")==0) {
		cout << "no such file!" << endl;
		waitKey(0);
	}


	while (1) {
		cap >> frame;
		if (frame.empty()) {
			break;
		}

		imshow("video", frame);
		double sec = cap.get(CAP_PROP_POS_MSEC);
		if (sec > 3000) {
			break;
		}

		int frames = cap.get(CAP_PROP_POS_FRAMES);
		int total_frames = cap.get(CAP_PROP_FRAME_COUNT);
		cout << "frames: " << frames << " / " << total_frames<<"\n";
		double fps = cap.get(CAP_PROP_FPS);
		waitKey(1000/fps);
		
	}




	waitKey(0);
}
