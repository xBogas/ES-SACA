#ifndef VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_
#define VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_

#include <QtCore/QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>  
#include "Approx.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

class Detector : public QObject
{
	Q_OBJECT

public:
	/// @brief Default Constructor
	/// @param type Target type 0 for Pistol and 1 for Rifle
	/// @param parent QObject parent
	explicit Detector(int type, int port, const char *addr, QObject *parent = nullptr);

	/// @brief Default Destructor
	~Detector(){};

	void
	onMain(bool& isRunning, bool& continueReading);

	void
	testMain()
	{
		getCapture();
		if(m_target == Target::Rifle)
			transformRifle();
		else
		{
			fakeTransform();
			curr_index++;
		}
		getCenter();
		getPoints();
	}


	void
	changeMode(int type);

signals: // This params should be a custom struct has they will always be sent together
	void new_score(double x, double y, double radius, double score);

private:
	enum class Target
	{
		Pistol,
		Rifle
	};

	enum TargetSize
	{
		Pistol = 170,
		Rifle = 100
	};


	/// @brief Calculate target center
	void
	getCenter();

	/// @brief Calculate mean of circles data where each element has (x,y,radius)
	/// @param data Vector with 3 channels per element
	/// @return Average values of first 2 channels (x,y)
	std::tuple<double, double> //! not used
	mean(const std::vector<cv::Vec3f> &data);

	/// @brief Calculates standard deviation of circle data
	/// @param data Circle data
	/// @param mean_x Average value of x circle
	/// @param mean_y Average value of y circle
	/// @return Deviation of x and y of data
	std::tuple<double, double> //! not used
	standardDeviation(const std::vector<cv::Vec3f> &data, double mean_x = 0, double mean_y = 0);

	/// @brief Reject circle data outliers
	/// @param data Circle data vector
	/// @param threshold Above this threshold all data is considered an outlier
	/// @return Average value of circle center
	std::tuple<int, int> //! not used
	rejectOutliers(const std::vector<cv::Vec3f> &data, int threshold);

	/// @brief Calculate mean of circles data where each element has (x,y,radius)
	/// @param data Vector with 3 channels per element
	/// @return Average circle center
	template<typename T>
	T
	mean(const std::vector<T> &data)
	{
		double sum_x = 0, sum_y = 0;
		for (size_t i = 0; i < data.size(); i++)
		{
			sum_x += data[i].x;
			sum_y += data[i].y;
		}
		
		return T(sum_x/data.size(), sum_y/data.size());
	}

	/// @brief Calculates standard deviation of data points
	/// @param data Points vector
	/// @param mean_x Average value of x
	/// @param mean_y Average value of y
	/// @return Deviation of x and y of data
	std::tuple<double, double> //! not used
	standardDeviation(const std::vector<cv::Point> &data, double mean_x, double mean_y);

	/// @brief Reject data points outliers
	/// @param data Points vector
	/// @param threshold Above this threshold all points are considered outliers
	/// @param output Points vector without outliers
	void //! not used
	rejectOutliers(const std::vector<cv::Point> &data, int threshold, std::vector<cv::Point> &output);

	/// @brief Shot detection algorithm
	void
	getPoints();

	/// @brief Score calculation
	double
	getScore(double distance, double radius);

	/// @brief Correct image perspective
	void
	transformImage();

	void
	transformRifle();

	void
	transformPistol();

	void
	getCapture();

	void
	orderCorners(cv::Mat& input, cv::Point2f* output);

	void
	fakeTransform();

	std::string
	getFile(int index)
	{
		switch (index)
		{
		case 0:
			return "226429_1.png";

		case 1:
			return "226435_1.png";

		case 2:
			return "226436_1.png";

		case 3:
			return "226439_1.png";

		default:
			return "226429_1.png";
		}
	}

	/// @brief Io handle
	boost::asio::io_context io_context;
	/// @brief Tcp socket
	tcp::socket socket;
	/// @brief Camera object
	cv::VideoCapture m_camera;
	/// @brief Ratio of pixel/mm
	float m_ratio;
	/// @brief Image object to analyze
	cv::Mat m_image;
	/// @brief Image reference
	cv::Mat m_img_ref;
	/// @brief Points Mat
	cv::Mat m_points;
	/// @brief Target center
	cv::Point2d m_center;
	/// @brief Target black circle radius
	double m_center_radius;
	/// @brief Approximation algorithm
	Approx m_approx;
	/// @brief Target type
	Target m_target;

	float m_lastScore;
	std::string m_path;
	int curr_index;
};

#endif
