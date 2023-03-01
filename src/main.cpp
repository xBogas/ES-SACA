#include <iostream>
#include <opencv2/opencv.hpp>

int main(int argc, char* argv[])
{
	if(argc != 2) 
	{
		std::cout << "Error! Program usage:" << std::endl;
		std::cout << "./main.exe image_path" << std::endl;
		exit(EXIT_FAILURE);
	}

	std::string path = argv[1];
	cv::Mat image = cv::imread(path);

	if (image.empty()) 
	{
		std::cout << "Error opening image\n";
		return EXIT_FAILURE;
	}

	cv::Mat gray;
    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);

    medianBlur(gray, gray, 5);
    
	std::vector<cv::Vec3f> circles;
    cv::HoughCircles(gray, circles, cv::HOUGH_GRADIENT, 1,
                 gray.rows/16,  // change this value to detect circles with different distances to each other
                 100, 30, 0, 50 // change the last two parameters
            // (min_radius & max_radius) to detect larger circles
    );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        cv::Vec3i c = circles[i];
        cv::Point center(c[0], c[1]);
        // circle center
        circle(image, center, 1, cv::Scalar(0,100,100), 3, cv::LINE_AA);
        // circle outline
        int radius = c[2];
        circle(image, center, radius, cv::Scalar(255,0,255), 3, cv::LINE_AA);
		std::cout << center << '\t' << radius << std::endl;
    }
    cv::imshow("detected circles", image);
    cv::waitKey();
    return EXIT_SUCCESS;
}
