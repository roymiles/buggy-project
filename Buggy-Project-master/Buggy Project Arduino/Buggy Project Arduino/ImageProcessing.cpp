#include "ImageProcessing.h"
#include <iostream>

ImageProcessing::ImageProcessing()
{
	ImageProcessing::floorToCameraDistance = 100;
}


ImageProcessing::~ImageProcessing()
{
}

Mat ImageProcessing::getImage() {
	Mat image;
	image = imread("C:\\Users\\Roy\\Desktop\\Uni 3rd Year\\blackwhite_intersect.png", IMREAD_COLOR); // Read the file

	if (!image.data) // Check for invalid input
	{
		cout << "Could not open or find the image" << std::endl;
		exit;
	}

	return image;
}

Mat ImageProcessing::convertToBW(Mat im_rgb) {
	Mat im_gray;
	cvtColor(im_rgb, im_gray, CV_RGB2GRAY);

	Mat img_bw = im_gray > 128;

	std::cout << "Displaying black and white image \n";
	//namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	//imshow("Display window", img_bw); // Show our image inside it.
	//waitKey(0); // Wait for a keystroke in the window

	return img_bw;
}

double ImageProcessing::getAngle(Mat img_bw) {
	cv::Size s = img_bw.size();

	std::cout << "Image is of height " << s.height << " and width " << s.width << "\n";

	// Generate two arbitrary points on the LHS of the image
	// They will both have the same x position but different y positions
	int x_start = s.width / 10;
	int y_start1 = (3 * s.height) / 4;
	int y_start2 = s.height / 4;

	// Get the colours of the starting pixels
	uchar pos1 = img_bw.at<uchar>(Point(x_start, y_start1));
	uchar pos2 = img_bw.at<uchar>(Point(x_start, y_start2));

	bool intsct1_flag = false;
	bool intsct2_flag = false;

	Point intsct1, intsct2;

	std::cout << "Starting sweep at p1(" << x_start << ", " << y_start1 << ") and p2(" << x_start << ", " << y_start2 << ")\n";
	std::cout << "p1:" << static_cast<unsigned>(pos1) << ", p2:" << static_cast<unsigned>(pos2) << "\n";

	Mat img_rgb;
	cvtColor(img_bw, img_rgb, CV_GRAY2BGR);

	// Keep moving along the image until a colour change occurs
	for (int x = x_start; x < s.width && (!intsct1_flag || !intsct2_flag); x++) {
		// Check first point
		// Only check if havent found intersection already
		if (!intsct1_flag){
			if (img_bw.at<uchar>(Point(x, y_start1)) != pos1) {
				// Change of colours
				intsct1_flag = true;
				std::cout << "p1:" << static_cast<unsigned>(pos1) << ", intsct:" << static_cast<unsigned>(img_bw.at<uchar>(Point(x, y_start1))) << "\n";
				std::cout << "p1 intsct(" << x << ", " << y_start1 << ")\n";
				intsct1 = Point(x, y_start1);
			}

			// Draw the route on the rgb image
			img_rgb.at<Vec3b>(Point(x, y_start1)) = Vec3b(255, 0, 0);
		}

		// Only check if havent found intersection already
		if (!intsct2_flag) {
			// Check second point
			if (img_bw.at<uchar>(Point(x, y_start2)) != pos2) {
				// Change of colours
				intsct2_flag = true;
				std::cout << "p2:" << static_cast<unsigned>(pos2) << ", intsct:" << static_cast<unsigned>(img_bw.at<uchar>(Point(x, y_start2))) << "\n";
				std::cout << "p2 intsct(" << x << ", " << y_start2 << ")\n";
				intsct2 = Point(x, y_start2);
			}

			// Draw the route on the rgb image
			img_rgb.at<Vec3b>(Point(x, y_start2)) = Vec3b(255, 0, 0);
		}
	}

	std::cout << "Drawing route \n";
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", img_rgb); // Show our image inside it.
	waitKey(0); // Wait for a keystroke in the window

	std::cout << "Found intersections at (" << intsct1.x << ", " << intsct1.y << ") and (" << intsct2.x << ", " << intsct2.y << ")\n";

	// Draw a green line (2px) about the line of intersection
	line(img_rgb, intsct1, intsct2,	Scalar(0, 255, 0), 2);

	std::cout << "Drawing over line of intersection \n";
	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display.
	imshow("Display window", img_rgb); // Show our image inside it.

	waitKey(0); // Wait for a keystroke in the window

	return 0;
}