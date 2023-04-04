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

	cv::GaussianBlur(gray, gray, cv::Size(5,5), 0);

	std::vector<cv::Vec3f> circles;
    HoughCircles(gray, circles, cv::HOUGH_GRADIENT_ALT, 1.5,
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 300, 0.85, 10, 100 // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center = cv::Point(c[0], c[1]);
        // circle center
        circle( m_image, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle( m_image, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
    }
    cv::imshow("detected circles", m_image);
    cv::waitKey();
}