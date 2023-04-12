#ifndef VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_
#define VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_

#include <QtCore/QObject>
#include <opencv2/opencv.hpp>

class Detector : public QObject
{
	Q_OBJECT

public:

	explicit Detector(QObject *parent = nullptr);
	
	~Detector(){};

private:

	void
	getCenter();
	
	std::tuple<double,double>
	mean(const std::vector<cv::Vec3f>& data);

	std::tuple<double,double>
	standartDeviation(const std::vector<cv::Vec3f>& data, double mean_x = 0, double mean_y = 0);

	std::tuple<int, int>
	rejectOutliers(const std::vector<cv::Vec3f>& data, int threshold);

	cv::VideoCapture m_camara;
	cv::Mat m_image;
};

#endif
