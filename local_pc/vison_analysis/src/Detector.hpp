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
	
	cv::VideoCapture m_camara;
	cv::Mat m_image;
};

#endif
