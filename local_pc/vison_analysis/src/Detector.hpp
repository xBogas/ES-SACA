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
	standardDeviation(const std::vector<cv::Vec3f>& data, double mean_x = 0, double mean_y = 0);

	std::tuple<int, int>
	rejectOutliers(const std::vector<cv::Vec3f>& data, int threshold);


	cv::Point
	mean(const std::vector<cv::Point> &data)
	{
		double sum_x = 0, sum_y = 0;
		for (size_t i = 0; i < data.size(); i++)
		{
			sum_x += data[i].x;
			sum_y += data[i].y;
		}
		
		return cv::Point(sum_x/data.size(), sum_y/data.size());
	}

	std::tuple<double,double>
	standardDeviation(const std::vector<cv::Point> &data, double mean_x, double mean_y)
	{
		double sum_x = 0, sum_y = 0;
		for (size_t i = 0; i < data.size(); i++){
			sum_x += pow(data[i].x - mean_x,2);
			sum_y += pow(data[i].y - mean_y,2);
		}
		return std::make_tuple(sqrt(sum_x/data.size()), sqrt(sum_y/data.size()));
	}

	void
	rejectOutliers(const std::vector<cv::Point>& data, int threshold, std::vector<cv::Point>& output)
	{
		auto [mean_x, mean_y] = mean(data);
    	auto [devi_x, devi_y] = standardDeviation(data, mean_x, mean_y);
		if ( devi_x == 0 || devi_y == 0)
			return ;
        
		int x = 0, y = 0, samples = 0;
		for (size_t i = 0; i < data.size(); i++)
		{
			if(std::abs(data[i].x - mean_x)/devi_x <= threshold 
			&& std::abs(data[i].y - mean_y)/devi_y <= threshold)
			{
				x += data[i].x;
				y += data[i].y;
				output.emplace_back(data[i].x,data[i].y);
				samples++;
			}
    	}
	}

	void
	getPoints();

	cv::VideoCapture m_camara;
	cv::Mat m_image;
	//std::vector<cv::Point2f> m_points;
	cv::Mat m_points;
};

#endif
