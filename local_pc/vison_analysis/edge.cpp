#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

const int RANGE = 50;

void getIntersections(std::vector<std::pair<cv::Point,cv::Point>>& lines)
{
	for (auto&& [first, second] : lines)
	{
		std::cout << "For line: " << first << " - " << second << "\n";

		if ((first.x - second.x) < (first.y - second.y))
		{
			std::cout << "This line is Horizontal\n";
		}
		else
			std::cout << "This line is Vertical\n";
	}
	
}

void filterLines(std::vector<cv::Vec2f>& input, std::vector<cv::Vec2f>& output)
{
	/**
	 * ? Search for transition areas to better select the correct line
	*/

	if (output.size() == 0)
		output.push_back(input[0]);

	int pos = 0;
	bool inside = false;
	for (auto &&i : input)
	{
		float i_rho = i[0];
		float i_theta = i[1];
		
		std::cout << "In pos: " << pos << "\n" 
				<< "rho is " << i_rho << " and theta is " << i_theta << "\n";
		pos++;
		for (auto &&j : output)
		{
			float o_rho = j[0];
			float o_theta = j[0];

			if (std::abs(o_rho - i_rho) < RANGE)
			{
				inside = true;
				continue;
			}				
		}
		if (!inside)
			output.push_back(i);

		inside = false;
	}
	std::cout << "Output\n";
	pos = 0;
	for (auto &&j : output)
	{
		std::cout << "In pos: " << pos << "\n" 
			<< "rho is " << j[0] << " and theta is " << j[1] << "\n";
		pos++;
	}	
}

int main(int argc, char const *argv[])
{
	const std::string IMAGPATH = "../images/img.jpg";
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

	std::vector<cv::Vec2f> lines, filtered;
    HoughLines(t_gauss, lines, 1, CV_PI/180, 400, 0, 0 );
	std::cout << "Found " << lines.size() << "\n";

	for (auto &&i : lines)
		std::cout << "Line: " << i << "\n";


	filterLines(lines, filtered);
	for (size_t i = 0; i < filtered.size(); i++)
	{
		float rho = filtered[i][0], theta = filtered[i][1];
		cv::Point pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));

		cv::line(src, pt1, pt2, cv::Scalar(255,0,0), 1, cv::LINE_AA);
	}
	cv::imshow("Filtered rho and theta", src);
	//exit(0);

	// TODO: Select intersections
	cv::waitKey(0);
	return 0;
}
