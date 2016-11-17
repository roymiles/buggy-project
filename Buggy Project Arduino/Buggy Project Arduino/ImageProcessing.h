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
<<<<<<< HEAD
	float floorToCameraDistance;
	const unsigned char red[3] = { 255,0,0 };
	const unsigned char green[3] = { 0,255,0 };
	const unsigned char blue[3] = { 0,0,255 };
=======
	Communications *c;

	// Get image from the arduino module
	CImg<unsigned char> getImage() {}
>>>>>>> 4512f29533787627fef620c274edc4d859907587

	// Extracts a small region of the image. This step will make it easier to extract the edges of the two metal beams.
	CImg<unsigned char> extractRegion(CImg<unsigned char>) {}
	
	// Convert the image to black and white for subsequent extraction of the edge coordinates using bwtraceboundary routine.
	CImg<unsigned char> thresholdImage(CImg<unsigned char>) {}
	
	// Find intersection by moving from given position untill color changes. This will need to be performed twice
	std::pair<int, int> traceToBoundary(int x, int y) {}
	
	// Find the angle of the line intersecting the two points
	double getAngle(std::pair<int, int>, std::pair<int, int>) {}
};

