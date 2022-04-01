#include<opencv2/imgcodecs.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/highgui.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//COLOR DETECTION
int hmin = 0, smin = 0, vmin = 0;
int hmax = 179, smax = 255, vmax = 255;

void main()
{
	Mat imgHSV, mask;

	namedWindow("Trackbars", (640, 200));
	createTrackbar("Hue Min", "Trackbars", &hmin, 179);
	createTrackbar("Hue Max", "Trackbars", &hmax, 179);
	createTrackbar("Sat Min", "Trackbars", &smin, 255);
	createTrackbar("Sat Max", "Trackbars", &smax, 255);
	createTrackbar("Val Min", "Trackbars", &vmin, 255);
	createTrackbar("Val Max", "Trackbars", &vmin, 255);

	VideoCapture cap(0);
	Mat img;

	while (true)
	{
		cap.read(img);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);

		Scalar lower(hmin, smin, vmin);//hue min, saturation min, value min
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);
		
		cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax<<endl;
		//found purple at 117,88,56,146,255,255
		//found green at 76,0,0,79,169,255
		//better green found at 62,130,123,104,187,255
		//orange: 0,93,122,22,182,255

		imshow("Image", img);
		imshow("Mask", mask);
		waitKey(1);
		

	}

}