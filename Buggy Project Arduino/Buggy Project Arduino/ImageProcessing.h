#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

#pragma once
class ImageProcessing
{
public:
	ImageProcessing();
	~ImageProcessing();

	// Get image from the arduino module
	Mat getImage();

	// Convert image to black and white
	Mat convertToBW(Mat image);

	// Get the angle of intersection between the squares from the image
	double getAngle(Mat image);

private:
	float floorToCameraDistance;
	const unsigned char red[3] = { 255,0,0 };
	const unsigned char green[3] = { 0,255,0 };
	const unsigned char blue[3] = { 0,0,255 };
};

