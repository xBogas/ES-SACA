#ifndef VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_
#define VISION_ANALYSIS_INCLUDE_DETECTOR_HPP_

#include <QtCore/QObject>
#include <opencv2/opencv.hpp>
#include "Approx.hpp"
#include "TCPsocket.hpp"

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
	onMain();

signals:
	void new_score(int x, int y, double radius, double score);

private:
	enum class Target
	{
		Pistol,
		Rifle
	};

	/// @brief Calculate target center
	void
	getCenter();

	/// @brief Calculate mean of circles data where each element has (x,y,radius)
	/// @param data Vector with 3 channels per element
	/// @return Average values of first 2 channels (x,y)
	std::tuple<double, double>
	mean(const std::vector<cv::Vec3f> &data);

	/// @brief Calculates standard deviation of circle data
	/// @param data Circle data
	/// @param mean_x Average value of x circle
	/// @param mean_y Average value of y circle
	/// @return Deviation of x and y of data
	std::tuple<double, double>
	standardDeviation(const std::vector<cv::Vec3f> &data, double mean_x = 0, double mean_y = 0);

	/// @brief Reject circle data outliers
	/// @param data Circle data vector
	/// @param threshold Above this threshold all data is considered an outlier
	/// @return Average value of circle center
	std::tuple<int, int>
	rejectOutliers(const std::vector<cv::Vec3f> &data, int threshold);

	/// @brief Calculate mean of circles data where each element has (x,y,radius)
	/// @param data Vector with 3 channels per element
	/// @return Average circle center
	cv::Point
	mean(const std::vector<cv::Point> &data);

	/// @brief Calculates standard deviation of data points
	/// @param data Points vector
	/// @param mean_x Average value of x
	/// @param mean_y Average value of y
	/// @return Deviation of x and y of data
	std::tuple<double, double>
	standardDeviation(const std::vector<cv::Point> &data, double mean_x, double mean_y);

	/// @brief Reject data points outliers
	/// @param data Points vector
	/// @param threshold Above this threshold all points are considered outliers
	/// @param output Points vector without outliers
	void
	rejectOutliers(const std::vector<cv::Point> &data, int threshold, std::vector<cv::Point> &output);

	/// @brief Shot detection algorithm
	void
	getPoints();

	double
	getScore(double distance);

	void transformImage();

	Network::TCPsocket *m_sock;
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
	Approx<double> m_approx;
	/// @brief Target type
	Target m_target;

	float m_lastScore;
};

#endif
