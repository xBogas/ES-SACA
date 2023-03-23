#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const int RANGE = 50;

void filterPoints(std::vector<std::pair<cv::Point,cv::Point>>& lines, const cv::Point& p1, const cv::Point& p2)
{
	if (lines.size() == 0)
	{
		lines.push_back(std::make_pair(p1,p2));
		return;
	}
	
	for (auto &&[first, second] : lines)
	{
		std::cout << "P1.x = " << p1.x << " \tfirst.x = " << first.x 
				<< "\tP1.y = " << p1.y << " \tfirst.y = " << first.y << "\n";
		if (first.x - RANGE < p1.x && p1.x < first.x + RANGE)
		{
			if (first.y - RANGE < p1.y && p1.y < first.y + RANGE)
			{
				std::cout << "Inside\n";
				return;
			}
		}
	}
	lines.push_back(std::make_pair(p1,p2));
}

void filterPoints_v2(std::vector<std::pair<cv::Point,cv::Point>>& lines, const cv::Point& p1, const cv::Point& p2)
{
	/**
	 * ? Search for transition areas to better select the correct line
	*/
}


int main(int argc, char const *argv[])
{
	const std::string IMAGPATH = "images/img.jpg";
	cv::Mat src = cv::imread(IMAGPATH, cv::IMREAD_COLOR);
	cv::Mat src_gray = cv::imread(IMAGPATH, cv::IMREAD_GRAYSCALE);
	cv::Mat t_bin, t_mean, t_gauss;

	blur(src_gray, src_gray, cv::Size(2,2));

	cv::threshold(src_gray, t_bin, 127, 255, cv::THRESH_BINARY);
	imshow("Global thresholding", t_bin);

	cv::adaptiveThreshold(src_gray, t_mean, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY_INV, 11, 2);
	imshow("Mean thresholding", t_mean);

	cv::adaptiveThreshold(src_gray, t_gauss, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 11, 4);
	imshow("Gauss thresholding", t_gauss);

	std::vector<cv::Vec2f> lines;
    HoughLines(t_gauss, lines, 1, CV_PI/180, 400, 0, 0 );
	std::cout << "Found " << lines.size() << "\n";


	std::vector<std::pair<cv::Point,cv::Point>> rec;
	rec.reserve(lines.size()*sizeof(std::pair<cv::Point,cv::Point>));

    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0], theta = lines[i][1];
        cv::Point pt1, pt2;
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
		filterPoints(rec, pt1, pt2);	// select better method to filer
        cv::line(src, pt1, pt2, cv::Scalar(0,0,255), 1, cv::LINE_AA);
    }
	cv::imshow("Detected Lines (in red) - Standard Hough Line Transform", src);
	
	cv::Mat src2 = cv::imread(IMAGPATH);
	std::cout << "Inside rec:\n";
	for (auto &&i : rec)
	{
		std::cout << "Pt1: " << i.first << " Pt2: " << i.second << "\n";
		cv::line(src2, i.first, i.second, cv::Scalar(0,255,0), 1, cv::LINE_AA);
	}
	cv::imshow("NAMES", src2);
	
	// TODO: Select intersections

	cv::waitKey(0);
	return 0;
}
