#include <Communications.h>
#include <Clmg.h>

using namespace cimg_library;

#pragma once
class ImageProcessing
{
public:
	ImageProcessing(Communications *c);
	~ImageProcessing();

private:
	Communications *c;

	// Get image from the arduino module
	CImg<unsigned char> getImage() {}

	// Extracts a small region of the image. This step will make it easier to extract the edges of the two metal beams.
	CImg<unsigned char> extractRegion(CImg<unsigned char>) {}
	
	// Convert the image to black and white for subsequent extraction of the edge coordinates using bwtraceboundary routine.
	CImg<unsigned char> thresholdImage(CImg<unsigned char>) {}
	
	// Find intersection by moving from given position untill color changes. This will need to be performed twice
	std::pair<int, int> traceToBoundary(int x, int y) {}
	
	// Find the angle of the line intersecting the two points
	double getAngle(std::pair<int, int>, std::pair<int, int>) {}
};

