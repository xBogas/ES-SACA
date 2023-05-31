#ifndef IMAGE_ALIGNMENT_H
#define IMAGE_ALIGNMENT_H

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2d.hpp>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
 

void alignImages(Mat &im1, Mat &im2, Mat &im1Reg, Mat &h);

#endif