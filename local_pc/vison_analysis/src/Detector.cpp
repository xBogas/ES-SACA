#include "Detector.hpp"
#include <vector>

#ifdef RELEASE
Detector::Detector(QObject *parent): QObject(parent), m_camara(0)
{

}
#else
Detector::Detector(QObject *parent): QObject(parent)
{
	std::cout << "I don't have a camera\n"
				<< "Running with test images\n";

	m_image = cv::imread("../images/pistol-align3.jpg", cv::IMREAD_COLOR);
	if (m_image.empty())
	{
		std::cerr << ("Failed to read image for testing");
		exit(EXIT_FAILURE);
	}
	
	cv::imshow("Test image", m_image);

	getCenter();
}
#endif


void Detector::getCenter()
{
	cv::Mat gray;
	cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);
    cv::imwrite("ref.jpg", m_image);
	cv::GaussianBlur(gray, gray, cv::Size(5,5), 0);

	std::vector<cv::Vec3f> circles;
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT_ALT, 1.5,
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 300, 0.98, 20, 500 // change the last two parameters (min_radius & max_radius) to detect larger circles
    );

    auto [x, y] = rejectOutliers(circles, 1);
    std::cout << "Circle at " << x << "," << y << std::endl;
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Point center = cv::Point(circles[i][0], circles[i][1]);
        circle( m_image, center, 1, cv::Scalar(0,255,0), 1, cv::LINE_AA);
        // circle outline
        int radius = circles[i][2];
        circle( m_image, center, radius, cv::Scalar(0,0,255), 1, cv::LINE_AA);
        std::cout << "Detected circle at " << center << " and radius " << radius << std::endl;
    }
    cv::imshow("detected circles", m_image);
    cv::waitKey();

    cv::imwrite("detected_circles.jpg", m_image);

}

std::tuple<double,double>
Detector::mean(const std::vector<cv::Vec3f> &data)
{
    double sum_x = 0, sum_y = 0;
    for (size_t i = 0; i < data.size(); i++)
    {
        sum_x += data[i][0];
        sum_y += data[i][1];
    }
    
	return std::make_tuple(sum_x/data.size(), sum_y/data.size());
}

std::tuple<double,double>
Detector::standartDeviation(const std::vector<cv::Vec3f> &data, double mean_x, double mean_y)
{
    double sum_x = 0, sum_y = 0;
    for (size_t i = 0; i < data.size(); i++){
        sum_x += pow(data[i][0] - mean_x,2);
        sum_y += pow(data[i][1] - mean_y,2);
    }
	return std::make_tuple(sqrt(sum_x/data.size()), sqrt(sum_y/data.size()));
}

std::tuple<int, int> 
Detector::rejectOutliers(const std::vector<cv::Vec3f> &data, int threshold)
{
    auto [mean_x, mean_y] = mean(data);
    auto [devi_x, devi_y] = standartDeviation(data, mean_x, mean_y);
    if ( devi_x == 0 || devi_y == 0)
        return std::make_tuple(mean_x, mean_y);
        
    int x = 0, y = 0, samples = 0;
	for (size_t i = 0; i < data.size(); i++)
    {
        if(std::abs(data[i][0] - mean_x)/devi_x <= threshold 
        && std::abs(data[i][1] - mean_y)/devi_y <= threshold)
        {
            x += data[i][0];
            y += data[i][1];
            samples++;
        }
    }
    
    return std::make_tuple(x/samples, y/samples);
}
