#include "Detector.hpp"
#include <vector>

#ifdef CAMERA
Detector::Detector(QObject *parent): QObject(parent), m_camera(0)
{

}
#else
Detector::Detector(QObject *parent)
	: QObject(parent),
	m_approx(525, 525, 14)
{
#ifdef DEBUG
	std::cout << "I don't have a camera\nRunning with test images\n";
#endif
	m_image = cv::imread("../images/sample1-align.jpg", cv::IMREAD_COLOR); 
	if (m_image.empty())
		throw std::runtime_error("Failed to read image for testing");
	else if (m_image.rows < 1020)
		throw std::runtime_error("Image size too low");
	
	m_ratio = m_image.rows/170.0f;

#ifdef DEBUG
	cv::imshow("Test image", m_image);
#endif
	getCenter();
	getPoints();
}
#endif

void Detector::getCenter()
{
	cv::Mat alt = m_image.clone(), edge;
	std::vector<cv::Vec3f> circles;
	

	std::vector<std::vector<cv::Point>> contours;
	cv::Canny(m_image, edge, 300, 500);
	cv::imshow("edge contours", edge);
	cv::findContours(edge, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 500)
		{
			double new_r = 185; //TODO: change /* 30 * 1050/170 */
			m_center_radius = new_r;
			double x_init = m_image.rows/2;
			double y_init = m_image.cols/2;

			m_approx.setInitialPoint(x_init, y_init);

			m_approx.insertPoints(contours[i]);
					
#ifdef DEBUG
			std::cout << "Set radius " << new_r << "\n";
#endif
			m_approx.setRadius(new_r);

			int iter = 0;
			while (iter < 100)
			{
				m_approx.updateJac();
				m_approx.updateF();
				m_approx.nextIter();
				//m_approx.print();
				iter++;

				auto [x,y] = m_approx.getCenter();
#ifdef DEBUG
			std::cout << "[" << iter << "]" << "Center " << x << "," << y << "\n";
#endif
			}

			m_approx.print();
			auto [x,y] = m_approx.getCenter();
			m_center.x = x, m_center.y = y;
			std::cout << "Center " << m_center << "\n";
			cv::circle(alt, cv::Point(x,y), 1, cv::Scalar(0,255,0), 1, cv::LINE_AA);
			cv::circle(alt, cv::Point(x,y), new_r, cv::Scalar(0,0,255), 1, cv::LINE_AA);
		}
	}
	cv::imshow("m_image circle", alt);

	
	cv::Mat gray;
	cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);

	blur(gray, gray, cv::Size(5,5) );
	HoughCircles(gray, circles, cv::HOUGH_GRADIENT_ALT, 1.5,
				gray.rows/16,  // change this value to detect circles with different distances to each other
				400, 0.99, 170, 200// change the last two parameters (min_radius & max_radius) to detect larger circles
	);

	for( size_t i = 0; i < circles.size(); i++ )
	{
		cv::Point center = cv::Point(circles[i][0], circles[i][1]);
		circle( m_image, center, 1, cv::Scalar(0,255,0), 1, cv::LINE_AA);
		// circle outline
		int radius = circles[i][2];
		circle( m_image, center, radius, cv::Scalar(0,0,255), 1, cv::LINE_AA);
//#ifdef DEBUG
		std::cout << "[Alt]Center at " << center << " and radius " << radius << std::endl;
//#endif
	}
//#ifdef DEBUG
	cv::imshow("[Alt]detected circles", m_image);
	cv::waitKey();
//#endif
	
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


cv::Point
Detector::mean(const std::vector<cv::Point> &data)
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
Detector::standardDeviation(const std::vector<cv::Vec3f> &data, double mean_x, double mean_y)
{
	double sum_x = 0, sum_y = 0;
	for (size_t i = 0; i < data.size(); i++){
		sum_x += pow(data[i][0] - mean_x,2);
		sum_y += pow(data[i][1] - mean_y,2);
	}
	return std::make_tuple(sqrt(sum_x/data.size()), sqrt(sum_y/data.size()));
}


std::tuple<double,double>
Detector::standardDeviation(const std::vector<cv::Point> &data, double mean_x, double mean_y)
{
	double sum_x = 0, sum_y = 0;
	for (size_t i = 0; i < data.size(); i++){
		sum_x += pow(data[i].x - mean_x,2);
		sum_y += pow(data[i].y - mean_y,2);
	}
	return std::make_tuple(sqrt(sum_x/data.size()), sqrt(sum_y/data.size()));
}


std::tuple<int, int> 
Detector::rejectOutliers(const std::vector<cv::Vec3f> &data, int threshold)
{
	auto [mean_x, mean_y] = mean(data);
	auto [devi_x, devi_y] = standardDeviation(data, mean_x, mean_y);
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


void
Detector::rejectOutliers(const std::vector<cv::Point>& data, int threshold, std::vector<cv::Point>& output)
{
	auto [mean_x, mean_y] = mean(data);
	auto [devi_x, devi_y] = standardDeviation(data, mean_x, mean_y);
	if ( devi_x == 0 || devi_y == 0)
		return ;
	
	int x = 0, y = 0, samples = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		if( std::abs(data[i].x - mean_x)/devi_x <= threshold &&
			std::abs(data[i].y - mean_y)/devi_y <= threshold)
		{
			x += data[i].x;
			y += data[i].y;
			output.emplace_back(data[i].x,data[i].y);
			samples++;
		}
	}
}


void Detector::getPoints()
{
	cv::Mat hsv;
	cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV_FULL); 
	
	// HSV color space
	// TODO: Correct background
	cv::Mat op2;
	blur(hsv, hsv, cv::Size(5,5) );
	cv::inRange(hsv, cv::Vec3b(100,0,120), cv::Vec3b(190,55,220), op2);
#ifdef DEBUG
	cv::imshow("Points hsv", op2);

	cv::Mat res1 = m_image.clone();
	cv::bitwise_and(m_image, cv::Scalar(0,0,255), res1, op2);

	cv::imshow("Img1", res1);
	cv::waitKey();
#endif

	// BGR color space
	/* cv::inRange(m_image, cv::Scalar(180,180,160), cv::Scalar(220,230,200), m_points);

	cv::Mat res2 = m_image.clone();
	cv::bitwise_and(m_image, cv::Scalar(0,0,255), res2, m_points);
	cv::imshow("Img2", res2);
	*/

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(op2, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

#ifdef DEBUG
	std::cout << "Found " << contours.size() << "\n";
#endif
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (cv::contourArea(contours[i]) > 300 && contours[i].size() < 700)
		{
			double new_r = 4.5/2.0f * 1050.0f/170.0f; //TODO: change
			double x_init = (contours[i][0].x + contours[i][20].x) / 2;
			double y_init = (contours[i][0].y + contours[i][20].y) / 2;
#ifdef DEBUG
			std::cout << "Set Init point  " << x_init << "," << y_init << "\n";
#endif
			m_approx.setInitialPoint(x_init, y_init);

			m_approx.insertPoints(contours[i]);
			
#ifdef DEBUG
			std::cout << "Set radius " << new_r << "\n";
#endif
			m_approx.setRadius(new_r);

			int iter = 0;
			while (iter < 100)
			{
				m_approx.updateJac();
				m_approx.updateF();
				m_approx.nextIter();
				//m_approx.print();
				iter++;

				auto [x,y] = m_approx.getCenter();
#ifdef DEBUG
				std::cout << "[" << iter << "]" << "Center " << x << "," << y << "\n";
#endif
			}
			m_approx.print();
			auto [x,y] = m_approx.getCenter();
			
			std::cout.setf(std::ios::fixed,std::ios::floatfield);
    		std::cout.precision(3);
			std::cout << "Center[" << i << "] (" << x << " , " << y << ") with " << cv::contourArea(contours[i]) << " contours area\n";
			cv::circle(m_image, cv::Point(x,y), new_r, cv::Scalar(255,0,0));

			cv::Point2d shot(x,y);

			double result = cv::norm(m_center-shot);
			std::cout << "Shot distance " << result << "\n";
			getScore(result);

			if (result > m_center_radius+15) // 185 -> center circle
			{
				std::cout << "Create mask for pixels at " << shot << "\n";
				std::cout << "ESP should not move\n";
			}
			else if (result < m_center_radius-15)
			{
				double move_ESP = m_center.y + std::sqrt(std::pow(m_center_radius,2) - std::pow(m_center.x - shot.x,2)) - shot.y;
				std::cout << "ESP should move " << move_ESP*170/1050 << " mm\n";
			}
			else
				std::cout << "Shot at limit\nMust mask and move ESP\n";
			
		}
	}
	cv::imshow("Shot detection", m_image);
	cv::waitKey();
}

void Detector::getScore(double distance)
{
	double score = 0;
	distance = abs(distance*170/1050 - 4.5/2);
	std::cout << "Distance " << distance << " mm\n";
	std::cout << "Score is ";
	if(distance <= 5.75)
	{
		score += 10;
		int dec = (5.75-distance)/(0.575);
		std::cout << score + dec*0.1 << "\n";
	}
	else if(distance <= 77.75)
	{
		double delta = 0.8;
		distance = 72-(distance-5.75);
		int score = distance/delta +10;
		std::cout << (float)score/(10.0f) << "\n";
	}
}


/**
 * 10 Ring: 5.75  mm
 * 9  Ring: 13.75 mm
 * 8  Ring: 21.75 mm
 * 7  Ring: 29.75 mm
 * 6  Ring: 37.75 mm
 * 5  Ring: 45.75 mm
 * 4  Ring: 53.75 mm
 * 3  Ring: 61.75 mm
 * 2  Ring: 69.75 mm
 * 1  Ring: 77.75 mm
 */

/**
 * 
 * 109 Ring: 2.50  mm
 * 100 Ring: 5.75  mm
 * 90  Ring: 13.75 mm
 * 80  Ring: 21.75 mm
 * 70  Ring: 29.75 mm
 * 60  Ring: 37.75 mm
 * 50  Ring: 45.75 mm
 * 40  Ring: 53.75 mm
 * 30  Ring: 61.75 mm
 * 20  Ring: 69.75 mm
 * 10  Ring: 77.75 mm
 */





/* struct DistFunc
{
	int dist;

	DistFunc(int _d) : dist(_d*_d) {}

	bool operator() (const cv::Point& p1, const cv::Point& p2) const
	{
		return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)) < dist;
	}
};
 */
// separate data
/* 	std::vector<cv::Point> data;
	cv::findNonZero(op2, data);
	int min_dist = 30;
	min_dist *= min_dist;
	std::vector<int> labels;

	int n_labels = partition(data, labels, 
		[min_dist](const cv::Point& p1, const cv::Point& p2){
			return ((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y)) < min_dist;
		});
	
	std::vector<std::vector<cv::Point>> separate_data(n_labels);

	for (size_t i = 0; i < labels.size(); i++)
		separate_data[labels[i]].push_back(data[i]); 
*/



/*
cv::Mat mask1(m_image.rows, m_image.cols, m_image.type(), cv::Scalar(0,0,0));
	cv::Mat mask2(m_image.rows, m_image.cols, m_image.type(), cv::Scalar(0,0,0));
	

	circle(mask2, p_center, 290,    cv::Scalar(255,255,255), -1, -1);
	circle(mask2, p_center, radius+3, cv::Scalar(0), -1, -1);
	circle(mask1, p_center, radius+3, cv::Scalar(255,255,255), -1, -1);

	cv::imshow("mask1", mask1);
	cv::imshow("mask2", mask2);

	cv::Mat center, donut;
	cv::bitwise_and(m_image, mask1, center);
	cv::bitwise_xor(center, mask1, center); // find closest to white

	cv::bitwise_and(m_image, mask2, donut);
	cv::bitwise_not(mask2, mask2);
	cv::bitwise_xor(donut, mask2, donut); // find closest to white
	
	cv::imshow("center", center);
	cv::imshow("donut", donut);

	// get center points
	cv::inRange(center, cv::Scalar(235,235,235), cv::Scalar(300,300,300), center);

	cv::imshow("center shot", center);
	std::vector<cv::Point> center_points;
	cv::findNonZero(center, center_points);
	
	std::vector<cv::Point> center_filter;
	
	rejectOutliers(center_points, 1, center_filter);

	for (auto &&i : center_filter)
	{
		std::cout << i << "\n";
	}
	

	// get donut points
	cv::inRange(donut, cv::Scalar(0,0,0), cv::Scalar(40,40,40), donut);

	cv::imshow("donut shot", donut);
	std::vector<cv::Point> donut_points;
	cv::findNonZero(donut, donut_points);
	
	std::vector<cv::Point> donut_filter;
	
	rejectOutliers(donut_points, 1, donut_filter);

	for (auto &&i : donut_filter)
	{
		std::cout << i << "\n";
	}
	cv::waitKey();
*/



/* 
	cv::Mat mask1(m_image.rows, m_image.cols, m_image.type(), cv::Scalar(0,0,0));
	cv::Mat mask2(m_image.rows, m_image.cols, CV_8UC1, cv::Scalar(0));
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	circle(mask1, (cv::Point(m_image.cols/2.f, m_image.rows/2.f) ), circles[0][2], cv::Scalar(255,255,255), -1, -1);

	circle(mask2, (cv::Point(m_image.cols/2.f, m_image.rows/2.f) ), 290,           cv::Scalar(255), -1, -1);
	circle(mask2, (cv::Point(m_image.cols/2.f, m_image.rows/2.f) ), circles[0][2], cv::Scalar(0), -1, -1);
	
	
	cv::imshow("mask1", mask1);
	cv::imshow("mask2", mask2);

	cv::Mat center, donut;
	cv::subtract(mask1, m_image, center);
	cv::subtract(m_image, cv::Scalar(100,0,0), donut, mask2);

	cv::imshow("center", center);
	cv::imshow("donut", donut);



	cv::Mat center_filtered, donut_filtered;

	cv::inRange(center, cv::Scalar(240,240,240), cv::Scalar(255,255,255), center_filtered);
	cv::inRange(donut,  cv::Scalar(0,0,0), cv::Scalar(50,50,50), donut_filtered);

	cv::imshow("center points", center_filtered);
	cv::imshow("donut filtered", donut_filtered);
 */












	//cv::waitKey();
	
	// slow method
	/* int distance = 15;

	int th2 = distance * distance; // squared radius tolerance
	std::vector<int> labels;

	std::cout<< "Starting \n";
	int n_labels = partition(m_points, labels, 
	[th2](const cv::Point& lhs, const cv::Point& rhs) { 
		return ((lhs.x - rhs.x)*(lhs.x - rhs.x) + (lhs.y - rhs.y)*(lhs.y - rhs.y)) < th2; 
	}
	);
	std::cout<< "Ended \n";
	std::vector<std::vector<cv::Point>> contours(n_labels);
	for (int i = 0; i < m_points.size(); ++i)
	{
		contours[labels[i]].push_back(m_points[i]);
	}
	std::cout << "Drawing\n";
	// Draw results

	// Build a vector of random color, one for each class (label)
	std::vector<cv::Vec3b> colors;
	for (int i = 0; i < n_labels; ++i)
	{
		colors.push_back(cv::Vec3b(rand() & 255, rand() & 255, rand() & 255));
	}

	// Draw the labels
	cv::Mat3b lbl(m_image.rows, m_image.cols, cv::Vec3b(0, 0, 0));
	for (int i = 0; i < m_points.size(); ++i)
	{
		lbl(m_points[i]) = colors[labels[i]];
	}

	imshow("Labels", lbl);
	cv::waitKey(); */
	/* std::vector<int> labels;

	cv::kmeans(m_points, 3, labels, 
		cv::TermCriteria(cv::TermCriteria::EPS, 1000, 0.0000001), 
		3, cv::KMEANS_RANDOM_CENTERS);

	

	out3.convertTo(out3, m_image.type());
	cv::cvtColor(out3, out3, cv::COLOR_GRAY2BGR);
	for (size_t i = 0; i < labels.size(); i++)
	{
		if (labels[i] == 0)
			out3.at<cv::Vec3b>(m_points[i]) = cv::Vec3b(255,0,0);
		else if (labels[i] == 1)
			out3.at<cv::Vec3b>(m_points[i]) = cv::Vec3b(0,255,0);
		else
			out3.at<cv::Vec3b>(m_points[i]) = cv::Vec3b(0,0,255);
		
	}

	imshow("final", out3); */



/*
	std::array<cv::Mat,3> channels;

	cv::split(hsv, channels);
	//cv::imshow("H channel", channels[0]);
	//cv::imshow("S channel", channels[1]);
	//cv::imshow("V channel", channels[2]);
	
	cv::Scalar h = cv::mean(hsv);
	std::cout << h << "\n";

	// HUE
	cv::threshold(channels[0], out1, h[0], h[0] + 10, cv::ThresholdTypes::THRESH_BINARY);
	cv::imshow("h ths", out1);
	
	cv::threshold(channels[0], out1, h[0], 255, cv::ThresholdTypes::THRESH_BINARY_INV);
	cv::imshow("h ths inv", out1);

	//Saturation
	cv::threshold(channels[1], out2, h[1], h[1] + 10, cv::ThresholdTypes::THRESH_BINARY);
	cv::imshow("s ths", out2);

	cv::threshold(channels[1], out2, h[1], 255, cv::ThresholdTypes::THRESH_BINARY_INV);
	cv::imshow("s ths inv", out2);


	cv::absdiff(h, channels[1], out2);
	
	cv::Scalar sat_mean, sat_std;
	cv::meanStdDev(hsv, sat_mean, sat_std);

	std::cout << "S mean: " << sat_mean << "\t S dev: " << sat_std << "\n";
	cv::inRange(hsv, sat_mean, sat_mean + 2*sat_std, out2);
	cv::imshow("s range", out2);


	// Value
	cv::threshold(channels[2], out3, h[2], h[2] + 20, cv::ThresholdTypes::THRESH_BINARY);
	cv::imshow("v ths", out3);

	cv::threshold(channels[2], out3, h[2], h[2] - 10, cv::ThresholdTypes::THRESH_BINARY_INV);
	cv::imshow("v ths inv", out3);



	
	cv::imshow("s diff", out1);
*/