#include "vision_analysis.h"

Detector::Detector()
{
}

void Detector::loadTemplate(const std::string &filename)
{
    m_template = cv::imread(filename, cv::IMREAD_GRAYSCALE);
}

int Detector::detectShot(cv::Mat &image)
{
    // TODO: Implement the shot detection and scoring algorithm
    return 0;
}