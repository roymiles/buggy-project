//#define cimg_use_jpeg
//#define HAVE_BOOLEAN
//#define XMD_H
//#define cimg_use_png
//#define cimg_use_magick
#include <Clmg.h>
//#include <Magick++.h>
#include <iostream> 

//using namespace Magick;
using namespace cimg_library;
using namespace std;

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <iostream>

using namespace cv;

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

	// Calculate the velocity of the buggy based on the correlation of two adjacent image frames
	double calculateVelocity() {}

	// Check if buggy is currently on a white square
	bool isOnWhiteSquare() {}

	// Check if buggy is currently on a black square
	bool isOnBlackSquare() {}

	// Gets the current square the buggy is on 'W' or 'B'
	char getCurrentSquare() {}
};

