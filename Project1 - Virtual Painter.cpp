#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<iostream>

using namespace std;
using namespace cv;

//cout << hmin << "," << smin << "," << vmin << "," << hmax << "," << smax << "," << vmax << endl;
vector<vector<int>> myColor{ {117,88,56,146,255,255},//purple
								{62,130,123,104,187,255}, //green
								{0,93,122,22,182,255} //Orange
							};
vector<Scalar> myColorValues{ {255, 0, 255}, //purple
							{0, 255, 0},//green
							{51, 153, 255} //Orange
							
							}; 
Mat img;
vector<vector<int>> newPoints;
//{{x, y, 0}, {x, y, 1}, {x, y, 1} }; 0 for purple, 1 for green, 2 for orance
Point getContours(Mat imgDil)
{
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//drawContours(img, contours, -1, Scalar(255, 0, 255), 10);
	////if its rectangle, it will have 4 points
	vector<vector<Point>> conPoly(contours.size());
	vector<Rect> boundRect(contours.size());
	
	Point myPoint(0, 0);

	for (int i = 0; i < contours.size(); i++)
	{
		int area = contourArea(contours[i]);
		cout << area << endl;
		string objectType;

		if (area > 1000)
		{
			//perimeter
			float peri = arcLength(contours[i], true);
			//find the number of corner point or curves that this polygon have. If polygon has 3 corner points then its triangle, if it has a lot then its circle etc
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);

			//finding the number of edges i.e. if edge 3 its triagle
			cout << conPoly[i].size() << endl;
			//to find the bounding rectange
			boundRect[i] = boundingRect(conPoly[i]);
			//to have point found from center and not width we do width/2
			myPoint.x = boundRect[i].x + boundRect[i].width / 2;
			myPoint.y = boundRect[i].y;

			drawContours(img, conPoly, i, Scalar(255, 0, 255), 10);
			rectangle(img, boundRect[i].tl(), boundRect[i].br(), Scalar(0, 255, 0), 5);
		}
	}
	return myPoint;
}

vector<vector<int>> findColor(Mat img)
{
	Mat imgHSV;
	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	for (int i = 0; i < myColor.size(); i++)
	{
		Scalar lower(myColor[i][0], myColor[i][1], myColor[i][2]);//hue min, saturation min, value min
		Scalar upper(myColor[i][3], myColor[i][4], myColor[i][5]);
		Mat mask;
		inRange(imgHSV, lower, upper, mask);
		//imshow(to_string(i), mask);
		Point myPoint=getContours(mask);
		if(myPoint.x!=0 && myPoint.y!=0)
			newPoints.push_back({ myPoint.x, myPoint.y, i});
	}
	return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues)
{
	for (int i = 0; i < newPoints.size(); i++)
	{
		circle(img, Point(newPoints[i][0], newPoints[i][1]), 10, myColorValues[newPoints[i][2]], FILLED);
	}

}

//WEBCAM
void main()
{
	//if there is only one camera => 0 is the value to put in VideoCapture cap(0). If there are other cams also, you can use 1 etc.
	VideoCapture cap(0);
	while (true)
	{
		cap.read(img);
		
		newPoints = findColor(img);
		drawOnCanvas(newPoints, myColorValues);
		imshow("Image", img); //at the end there will always be an error as there are no more images to show. 
		//Since we are learning, it is okay. We know how to code and read+display video
		//We dont want to wait for infinity as its video. So we can set waitKey for 1ms i.e. waitKey(1). If we want it slower
		// we can set as waitKey(5);
		waitKey(1);
	}
}