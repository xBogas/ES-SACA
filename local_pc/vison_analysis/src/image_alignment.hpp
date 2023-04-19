#ifndef IMAGE_ALIGNMENT_H
#define IMAGE_ALIGNMENT_H

#include <opencv2/opencv.hpp>
#include <opencv2/xfeatures2d.hpp>
#include <opencv2/features2>

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
 
const int MAX_FEATURES = 1000;
const float GOOD_MATCH_PERCENT = 0.15f;

void alignImages(Mat &im1, Mat &im2, Mat &im1Reg, Mat &h);

#endif