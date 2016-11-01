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
	float floorToCameraDistance;

	// Get image from the arduino module
	CImg<unsigned char> getImage() {}

	// Calculate the velocity of the buggy based on the correlation of two adjacent image frames
	double calculateVelocity() {}

	// Check if buggy is currently on a white square
	bool isOnWhiteSquare() {}

	// Check if buggy is currently on a black square
	bool isOnBlackSquare() {}

	// Gets the current square the buggy is on 'W' or 'B'
	char getCurrentSquare() {}
};

