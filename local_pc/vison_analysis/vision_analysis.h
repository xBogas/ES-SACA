#ifndef VISION_ANALYSIS_H
#define VISION_ANALYSIS_H

#include <QtCore/QObject>
#include <opencv2/opencv.hpp>
#include <thread>
#include <condition_variable>

class Detector : public QObject
{
	Q_OBJECT

public:

	explicit Detector(QObject *parent = nullptr);
	
	~Detector();

private:

	void getImage();

	bool correctPerspective();

	bool detect();
	
	cv::VideoCapture m_camara;
	cv::Mat m_image;
};

#endif // DETECTOR_H
