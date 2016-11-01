#include "ImageProcessing.h"

ImageProcessing::ImageProcessing(Communications *c)
{
	ImageProcessing::c = c;
	ImageProcessing::floorToCameraDistance = 100;
}


ImageProcessing::~ImageProcessing()
{
}
