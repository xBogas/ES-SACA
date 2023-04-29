#ifndef VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_
#define VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_

#include <QtCore/QObject>
#include <opencv2/opencv.hpp>
#include "Approx.hpp"

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
	standardDeviation(const std::vector<cv::Vec3f>& data, double mean_x = 0, double mean_y = 0);


	std::tuple<int, int>
	rejectOutliers(const std::vector<cv::Vec3f>& data, int threshold);


	cv::Point
	mean(const std::vector<cv::Point> &data);


	std::tuple<double,double>
	standardDeviation(const std::vector<cv::Point> &data, double mean_x, double mean_y);


	void
	rejectOutliers(const std::vector<cv::Point>& data, int threshold, std::vector<cv::Point>& output);


	void
	getPoints();


#ifdef CAMERA
	cv::VideoCapture m_camera;
#endif
	// in millimeters/pixel
	float m_ratio;
	cv::Mat m_image;
	//std::vector<cv::Point2f> m_points;
	cv::Mat m_points;
	cv::Point2d m_center;
	Approx<double>* m_approx;
};

#endif
