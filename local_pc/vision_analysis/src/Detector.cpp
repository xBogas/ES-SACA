#include "Detector.hpp"
#include <vector>
#include <chrono>


// #define CAMERA
Detector::Detector(int type, int port, const char* addr, QObject *parent)
	: QObject(parent), m_approx(525, 525), socket(io_context), curr_index(0)
{
	switch (type)
	{
	case 0:
		m_target = Target::Pistol;
		m_img_ref = cv::imread("../images/pistol_ref.jpg");
		break;
	
	case 1:
		m_target = Target::Rifle;
		m_img_ref = cv::imread("../images/rifle_ref.jpg");
		break;

	default:
		throw std::runtime_error("Invalid target type");
		break;
	} 

	// Connect to ESP8266
#ifdef ESP_COMS
	tcp::resolver resolver(io_context);
  	boost::asio::connect(socket, resolver.resolve(addr, std::to_string(port))); // ESP8266 IP address and port
	boost::asio::write(socket, boost::asio::buffer(""));
	std::cout << "[Connected to ESP8266]" << std::endl;
#endif

#ifdef CAMERA
	if(!m_camera.open("/dev/video0"))
		throw std::runtime_error("Failed to open camera");
	m_camera.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	m_camera.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
#endif
}

void Detector::onMain(bool& finish, bool& continueReading)
{
	char buffer[1024];
    boost::system::error_code error;
	bool newRead = false, oldRead = false, changePhoto = false;
	while (!finish)
	{
		std::cout << "[Waiting for data...]" << std::endl;
		size_t length = socket.read_some(boost::asio::buffer(buffer), error);
		if (error == boost::asio::error::eof) {
			std::cout << "Connection closed by server." << std::endl;
			break;
		} else if (error) {
			std::cout << "Error: " << error.message() << std::endl;
			break;
		}
		if(continueReading){
			std::string message(buffer, length);
			std::cout << "Response: " << message << std::endl;
			
			newRead = (std::strncmp(buffer, "disparo", std::strlen("disparo")) == 0);
			//newRead = message.size() >= 4;
			if (newRead)
			{
				if (m_target == Target::Pistol)
				{
					testMain(changePhoto);
					changePhoto = !changePhoto;
				}
				else
				{
					getCapture();
					if (m_target == Target::Pistol)
						transformImage(); // transformPistol();
					else
						transformRifle();
					getCenter();
					getPoints();
				}
			}
			//oldRead = newRead;
		}
		else{
			boost::asio::write(socket, boost::asio::buffer("NO\n"));
		}
	}
}

void Detector::changeMode(int type)
{
	switch (type)
	{
	case 0:
		m_target = Target::Pistol;
		m_img_ref = cv::imread("../images/pistol_ref.jpg");
		break;
	
	case 1:
		m_target = Target::Rifle;
		m_img_ref = cv::imread("../images/rifle_ref.jpg");
		break;

	default:
		throw std::runtime_error("Invalid target type");
		break;
	}
}

void Detector::getCenter()
{
	cv::Mat alt = m_image.clone(), edge;
	std::vector<cv::Vec3f> circles;

	std::vector<std::vector<cv::Point>> contours;
	//cv::cvtColor(alt, alt, cv::COLOR_BGR2GRAY);
	//cv::blur(alt, alt, cv::Size(5,5));
	cv::Canny(m_image, edge, 200, 500); // for pistol 200, 500
#if defined(VISION_TEST) && defined(DEBUG)
	cv::imshow("edge contours", edge);
	cv::imwrite("edge.png", edge);
	cv::waitKey();
#endif
	if(m_target == Target::Pistol)
		cv::findContours(edge, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	else
		cv::findContours(edge, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
	std::vector<cv::Point2d> centers;

	for (size_t i = 0; i < contours.size(); i++)
	{
		// above 200 for pistol -> 1050
		// above 950 for rifle -> 1190

		/* if(contours[i].size() > 100'000 || contours[i].size() < 40)
			continue;

		cv::Mat alt = m_image.clone();
		cv::drawContours(alt, contours, i, cv::Scalar(0, 0, 255), 2);
		cv::imshow("contour's", alt);
		cv::waitKey();
		std::cout << "[" << i << "] size " << contours[i].size() << " and " << cv::contourArea(contours[i]) << " contours area\n"; */

		double min_area = (m_target == Target::Pistol)? 70'000 : 100;
		double min_size = m_target == Target::Pistol? 300 : 1'000;
		if (contours[i].size() > 300 && cv::contourArea(contours[i]) > min_area)
		{
			if (m_target == Target::Pistol)
				m_center_radius = 210;
			else
				m_center_radius = 182;
			double x_init = m_image.rows/2;
			double y_init = m_image.cols/2;

			m_approx.setInitialPoint(x_init, y_init);
			m_approx.setRadius(m_center_radius);

			m_approx.insertPoints(contours[i]);

#ifdef DEBUG
			std::cout << "Set radius " << m_center_radius << "\n";
#endif

			int iter = 0;
			std::cout << "[" << iter << "]" << "Center " << x_init << "," << y_init << "\n";
			while (iter < 5)
			{
				m_approx.updateJac();
				m_approx.updateF();
				m_approx.nextIter();
				iter++;

#ifdef DEBUG
			auto [x,y] = m_approx.getCenter();
			std::cout << "[" << iter << "]" << "Center " << x << "," << y << "\n";
#endif
			}

			auto [x,y] = m_approx.getCenter();
			m_center.x = x, m_center.y = y;
			centers.emplace_back(x, y);
#if defined(VISION_TEST) && defined(DEBUG)
			std::cout << "Center " << m_center << " and radius " << m_center_radius << " " << contours[i].size() << " and area " << cv::contourArea(contours[i]) << "\n";

			cv::circle(alt, m_center, m_center_radius, cv::Scalar(0, 0, 255), 2);
			cv::imwrite("center.png", alt);
			cv::imshow("Center detection", alt);
			cv::waitKey();
			alt = m_image.clone();

			cv::circle(alt, m_center, 0, cv::Scalar(0, 0, 255));
			cv::drawContours(alt, contours, i, cv::Scalar(0, 255, 0));
			cv::imshow("Center detection", alt);
			cv::waitKey();
			alt = m_image.clone();
			m_approx.print();
#endif
		}
	}
	m_center = mean(centers);
#ifdef VISION_TEST
	std::cout << "Avg center " << m_center << " and radius " << m_center_radius << "\n";
	cv::destroyAllWindows();
#endif
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


/* cv::Point
Detector::mean(const std::vector<cv::Point> &data)
{
	double sum_x = 0, sum_y = 0;
	for (size_t i = 0; i < data.size(); i++)
	{
		sum_x += data[i].x;
		sum_y += data[i].y;
	}
	
	return cv::Point(sum_x/data.size(), sum_y/data.size());
} */


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


void 
Detector::getPoints()
{
	cv::Mat hsv, display;
#ifdef VISION_TEST
	display = m_image.clone();
#endif
	cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV_FULL); 

	// HSV color space
	cv::Mat op2;
	blur(hsv, hsv, cv::Size(5,5));
	if (m_target == Target::Rifle)
		cv::inRange(hsv, cv::Vec3b(40,0,240), cv::Vec3b(255,120,255), op2); //TODO: Check contours
	else
		cv::inRange(hsv, cv::Vec3b(0,0,240), cv::Vec3b(255,255,255), op2);
		

#ifdef DEBUG
	cv::Mat chann[3];
	cv::split(hsv, chann);
	cv::imshow("H_cut", chann[0]);
	cv::imwrite("H_cut.png", chann[0]);
	
	cv::imshow("S_cut", chann[1]);
	cv::imwrite("S_cut.png", chann[1]);

	cv::imshow("V_cut", chann[2]);
	cv::imwrite("V_cut.png", chann[2]);

	cv::imshow("Points hsv", op2);
	

	cv::Mat res1 = m_image.clone();
	cv::bitwise_and(m_image, cv::Scalar(0,0,255), res1, op2);

	cv::imshow("Img1", res1);
	cv::imwrite("points.png", res1);
	cv::waitKey();
#endif

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(op2, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

#ifdef DEBUG
	std::cout << "Found " << contours.size() << "\n";
	cv::destroyAllWindows();
#endif

	for (size_t i = 0; i < contours.size(); i++)
	{
		double area = cv::contourArea(contours[i]);
		double max_area = (m_target == Target::Pistol)? 900 : 7'000;
		double min_area = (m_target == Target::Pistol)? 400 : 1'700;
		int max_size = (m_target == Target::Pistol)? 100 : 220;
		/* if(contours[i].size() > 200 || contours[i].size() < 40)
			continue;

		cv::Mat alt = m_image.clone();
		cv::drawContours(alt, contours, i, cv::Scalar(0, 0, 255), 2);
		cv::imshow("contour's", alt);
		cv::waitKey();
		std::cout << "[" << i << "] size " << contours[i].size() << " and " << cv::contourArea(contours[i]) << " contours area\n"; */

		//TODO: Check contours
		if ( area > min_area && area < max_area /*&& contours[i].size() < max_size && contours[i].size() > 60 */)
		{
			//? 16 for pistol		4.5/2 * 1190/170;
			//? 27 for rifle		4.5/2 * 1190/100
			double shot_radius = (m_target == Target::Pistol)? 16 : 27;

			double x_init = (contours[i][0].x + contours[i][20].x) / 2;
			double y_init = (contours[i][0].y + contours[i][20].y) / 2;

#ifdef DEBUG
			std::cout << "Set Init point  " << x_init << "," << y_init << "\n";
#endif
			m_approx.setInitialPoint(x_init, y_init);
			m_approx.setRadius(shot_radius);
			m_approx.insertPoints(contours[i]);
			
#ifdef DEBUG
			std::cout << "Set radius " << shot_radius << "\n";
#endif

			int iter = 0;
			while (iter < 5)
			{
				m_approx.updateJac();
				m_approx.updateF();
				m_approx.nextIter();
				iter++;

#ifdef DEBUG
				auto [x,y] = m_approx.getCenter();
				std::cout << "[" << iter << "]" << "Center " << x << "," << y << "\n";
#endif

			}
			auto [x,y] = m_approx.getCenter();

#if defined(VISION_TEST) && defined(DEBUG)
			std::cout.setf(std::ios::fixed,std::ios::floatfield);
    		std::cout.precision(3);
			std::cout << "Center[" << i << "] (" << x << " , " << y << ") with radius " << shot_radius << " and "<< cv::contourArea(contours[i]) << " contours area\n";
			cv::circle(display, cv::Point(x,y), shot_radius, cv::Scalar(0,0,255), 2);
			cv::circle(display, cv::Point(x,y), 0, cv::Scalar(0,0,255), 2);
			cv::imshow("Shot detection", display);
			cv::imwrite("Shot.png", display);
			cv::waitKey();
#endif

			cv::Point2d shot(x,y);

			double distance = cv::norm(m_center - shot);
			double angle = atan2(-1*(shot.y-m_center.y), (shot.x-m_center.x));
			
			
			std::cout << "Shot distance " << distance << " pixels and angle " << angle << "\n";
			double score = getScore(distance, shot_radius);

			if ((score == m_lastScore) && (m_target == Target::Pistol))
			{
				std::cout << "Same as before not sending\n";
				continue;
			}
			else
				m_lastScore = score;

			emit new_score(distance*930/1190, angle, shot_radius, score);
			if (distance > m_center_radius+shot_radius) // 185 -> center circle
			{	
				std::cout << "Create mask for pixels at " << shot << "\n";
				std::cout << "ESP should not move\n";
#ifdef ESP_COMS
				boost::asio::write(socket, boost::asio::buffer("NO\n"));
#endif
			}
			else if (distance < m_center_radius-shot_radius)
			{
				float move_ESP = m_center.y + std::sqrt(std::pow(m_center_radius,2) - std::pow(m_center.x - shot.x,2)) - shot.y + shot_radius;
				if (m_target == Target::Pistol)
					move_ESP *= 170.0f/1190.0f;

				else
					move_ESP *= 100.0f/1190.0f;

				std::string msg = "MOVE " + std::to_string(move_ESP) + "\n";

				std::cout << msg << std::endl;
				std::cout << "[Sending data...]" << std::endl;
#ifdef ESP_COMS
				boost::asio::write(socket, boost::asio::buffer(msg));
#endif
				std::cout << "ESP should move " << move_ESP << " mm\n";
			}
			else
			{
				std::cout << "Shot at limit\nMust mask and move ESP\n";

				float move_ESP = m_center.y + std::sqrt(std::pow(m_center_radius,2) - std::pow(m_center.x - shot.x,2)) - shot.y + shot_radius;

				if (m_target == Target::Pistol)
					move_ESP *= 170.0f/1190.0f;

				else
					move_ESP *= 100.0f/1190.0f;
				
				std::string msg = "MOVE " + std::to_string(move_ESP) + "\n";
				std::cout << msg << std::endl;
				std::cout << "[Sending data...]" << std::endl;
				std::cout << "ESP should move " << move_ESP << " mm\n";
#ifdef ESP_COMS
				boost::asio::write(socket, boost::asio::buffer(msg));
#endif
			}
			return;
		}
	}
#if defined(VISION_TEST) && defined(DEBUG)
	cv::imshow("Shot detection", display);
	cv::waitKey();
#endif
}

//!Review for Pistol
double Detector::getScore(double distance, double radius)
{
	double score = 0;
	if (m_target == Target::Pistol)  //! Review
	{
		distance = (distance-radius)*TargetSize::Pistol/m_image.cols;
		if(distance <= 5.75)
		{
			score += 10;
			int dec = (5.75-distance)/(0.575);
			score = score + dec*0.1;
			std::cout << score + dec*0.1 << "\n";
		}
		else if(distance <= 77.75)
		{
			double delta = 0.8;
			distance = 72-(distance-5.75);
			int sc = distance/delta +10;
			score = (float)sc/(10.0f);
		}
	}
	else
	{
		distance = (distance-radius)*TargetSize::Rifle/m_image.cols;
		std::cout << "Distance " << distance << " mm\n";
		if(distance <= 0.25)
		{
			if (distance < 0)
			{
				score = 10.9f;
			}	
			else
			{
				score = 10;
				int dec = (0.25-distance)/(0.025);
				score = score + dec*0.1;
				std::cout << score + dec*0.1 << "\n";
			}
		}
		else if(distance <= 22.75)
		{
			double delta = 0.25;
			distance = 22.5-(distance-0.25);
			std::cout << "Distance to 1.0 " << distance << " mm\n";
			int sc = distance/delta +10;
			score = (float)sc/(10.0f);
		}
	}
	std::cout << "Score is ";
	std::cout << "\033[1;32m" << score << "\033[0m\n";
	return score;
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

//! For pistol only
void Detector::transformImage()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	
	const int MAX_FEATURES = 10000;
	const float GOOD_MATCH_PERCENT = 0.15f;

	// Convert images to grayscale
	cv::Mat im1Gray, im2Gray;
	cvtColor(m_image, im1Gray, cv::COLOR_BGR2GRAY);
	cvtColor(m_img_ref, im2Gray, cv::COLOR_BGR2GRAY);

	// Variables to store key points and descriptors
	std::vector<cv::KeyPoint> key_points1, key_points2;
	cv::Mat descriptors1, descriptors2;

	// Detect ORB features and compute descriptors.
	cv::Ptr<cv::Feature2D> orb = cv::ORB::create(MAX_FEATURES);
	orb->detectAndCompute(im1Gray, cv::Mat(), key_points1, descriptors1);
	orb->detectAndCompute(im2Gray, cv::Mat(), key_points2, descriptors2);

	// Match features.
	std::vector<cv::DMatch> matches;
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce-Hamming");
	matcher->match(descriptors1, descriptors2, matches);
	

	//? (1) Sorting but only matters the highest matches
	//TODO: get matches.size() * GOOD_MATCH_PERCENT highest without sorting all of the vector
	std::sort(matches.begin(), matches.end());
	
	// Remove not so good matches
	const int numGoodMatches = matches.size() * GOOD_MATCH_PERCENT;
	matches.erase(matches.begin()+numGoodMatches, matches.end());
	//? (1)

#ifdef DEBUG
	cv::Mat imMatches;
	drawMatches(m_image, key_points1, m_img_ref, key_points2, matches, imMatches);
	cv::imwrite("matches.jpg", imMatches);
#endif
	// Extract location of good matches
	std::vector<cv::Point2f> points1, points2;
	points1.reserve(matches.size());
	points2.reserve(matches.size());
	
	for( size_t i = 0; i < matches.size(); i++ )
	{
		points1.push_back(key_points1[matches[i].queryIdx].pt);
		points2.push_back(key_points2[matches[i].trainIdx].pt);
	}

	// Find homography
	cv::Mat h = findHomography(points1, points2, cv::RANSAC);

	// Use homography to warp image
	warpPerspective(m_image, m_image, h, m_img_ref.size());
	end = std::chrono::high_resolution_clock::now();
	std::cout << "TransformImage: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs\n";
#ifdef DEBUG
	cv::imshow("Transformed Image",m_image);
	cv::waitKey();
	cv::imwrite("transformed.jpg", m_image);
#endif
}


void Detector::transformRifle()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();

	//get 4 corner points of m_image
	std::cout << "transformRifle()\n";
	cv::Mat hsv, corners;
	cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV_FULL); 
	cv::blur(hsv, hsv, cv::Size(5,5));	

	cv::Mat chann[3];
	cv::split(hsv, chann);
#ifdef DEBUG
	cv::imshow("H", chann[0]);
	cv::imwrite("H.png", chann[0]);
	
	cv::imshow("S", chann[1]);
	cv::imwrite("S.png", chann[1]);

	cv::imshow("V", chann[2]);
	cv::imwrite("V.png", chann[2]);
#endif

	cv::Mat op2;
	blur(hsv, hsv, cv::Size(3, 3));
	//TODO: fix hsv range
	cv::inRange(hsv, cv::Vec3b(120,0,0), cv::Vec3b(255,85,255), op2);
	//cv::inRange(chann[0], cv::Scalar(55), cv::Scalar(255), op2);

#ifdef DEBUG
	cv::imshow("Filtered", op2);
	cv::waitKey();
	cv::imwrite("Captured.png", m_image);
#endif
	/* 
	cv::Mat morph, kernel;
	kernel = cv::Mat(3, 3, CV_8U, cv::Scalar(1));
	cv::morphologyEx(op2, morph, cv::MORPH_ERODE, kernel);
	cv::imshow("Kernel", morph);
	cv::waitKey();
	*/

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(op2, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	std::cout << "Found " << contours.size() << "\n";
	double max_area = 0;
	int pos = 0;
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() < 700)
			continue;

		double area = cv::contourArea(contours[i]);
		if( area < 10'000)
			continue;
		if (area > max_area)
		{
			max_area = area;
			pos = i;
		}
#if defined(DEBUG)
		cv::Mat display = m_image.clone();
		std::cout << "Area " << area << " and size " << contours[i].size() << "\n";
		cv::drawContours(display, contours, i, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
		cv::imshow("Image", display);
		cv::waitKey();
#endif
	}

	cv::approxPolyDP(contours[pos], corners, 0.05*cv::arcLength(contours[pos], true), true);

#if defined(DEBUG)
	std::cout << "Out: " << corners << "\n";
	cv::drawContours(m_image, corners, -1, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	cv::imshow("Image", m_image);
	cv::waitKey();

	std::vector<cv::Point> corner_points;
	corner_points.push_back(corners.at<cv::Point>(0));
	corner_points.push_back(corners.at<cv::Point>(1));
	corner_points.push_back(corners.at<cv::Point>(2));
	corner_points.push_back(corners.at<cv::Point>(3));

	const cv::Point* point = &corner_points[0];
	int n = corner_points.size();
	cv::Mat draw = m_image.clone();
	cv::polylines(draw, &point, &n, 1, true, cv::Scalar(0, 255, 0), 3, cv::LINE_AA);
	imwrite("TargetContour.jpg", draw);
#endif

	cv::Point2f src_vertices[4];
	orderCorners(corners, src_vertices);



	cv::Point2f dst_vertices[4];
	dst_vertices[0] = cv::Point(0, 0);
	dst_vertices[1] = cv::Point(0, 1190);
	dst_vertices[2] = cv::Point(1190, 1190);
	dst_vertices[3] = cv::Point(1190, 0);

	cv::Mat warpPerspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	warpPerspective(m_image, m_image, warpPerspectiveMatrix, cv::Size(1190, 1190), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	end = std::chrono::high_resolution_clock::now();
	std::cout << "TransformRifle: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs\n";

#if defined(DEBUG)
	cv::imshow("Transform result", m_image);
	cv::imwrite("transformed.png", m_image);
	cv::waitKey();
#endif
}



void Detector::transformPistol()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();

	cv::Mat gray, hsv, corners;
	cv::cvtColor(m_image, hsv, cv::COLOR_BGR2HSV_FULL);
	cv::cvtColor(m_image, gray, cv::COLOR_BGR2GRAY);
	cv::bilateralFilter(m_image.clone(), m_image, 5, 150, 150);
	cv::Mat chann[3], color;
	cv::split(hsv, chann);

	cv::imshow("H", chann[0]);
	cv::imshow("S", chann[1]);
	cv::imshow("V", chann[2]);
	cv::imshow("gray", gray);

	cv::threshold(chann[1], gray, 20, 255, cv::THRESH_BINARY_INV);
	cv::imshow("gray 2", gray);


	cv::inRange(m_image, cv::Scalar(150, 0, 0), cv::Scalar(255, 255, 255), color);
	cv::imshow("filter blue", color);

	cv::inRange(hsv, cv::Scalar(150, 0, 0), cv::Scalar(255, 255, 255), color);
	cv::imshow("filter hue", color);
	cv::waitKey();

	cv::Mat kernel = cv::Mat(3, 3, CV_8U, cv::Scalar(1));
	cv::morphologyEx(color, color, cv::MORPH_ERODE, kernel);
	cv::imshow("morph hue", color);
	cv::waitKey();
	
	std::vector<std::vector<cv::Point>> contours, merged_contour(1);
	cv::findContours(color, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
	for (size_t i = 0; i < contours.size(); i++)
	{
		if (contours[i].size() > 40)
		{
			merged_contour[0].reserve(contours[i].size());
			for (size_t j = 0; j < contours[i].size(); j++)
				merged_contour[0].push_back(contours[i][j]);
		}
	}
	
	cv::Mat display = m_image.clone();
	cv::drawContours(display, merged_contour, 0, cv::Scalar(0, 255, 0), 2, cv::LINE_AA);
	cv::imshow("merged", display);
	cv::waitKey();
	cv::approxPolyDP(merged_contour[0], corners, 500, true);

	cv::Point2f src_vertices[4];
	orderCorners(corners, src_vertices);

	cv::Point2f dst_vertices[4];
	dst_vertices[0] = cv::Point(0, 0);
	dst_vertices[1] = cv::Point(0, 1190);
	dst_vertices[2] = cv::Point(1190, 1190);
	dst_vertices[3] = cv::Point(1190, 0);

	cv::Mat warpPerspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	warpPerspective(m_image, m_image, warpPerspectiveMatrix, cv::Size(1190, 1190), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	end = std::chrono::high_resolution_clock::now();
	std::cout << "TransformRifle: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs\n";

#if defined(DEBUG)
	cv::imshow("Transform result", m_image);
	cv::imwrite("transformed.png", m_image);
	cv::waitKey();
#endif
}

void Detector::getCapture()
{
#ifdef CAMERA
	m_camera.release();
	if(!m_camera.open("/dev/video0"))
		throw std::runtime_error("Failed to open camera");
	m_camera.set(cv::CAP_PROP_FRAME_WIDTH, 1920);
	m_camera.set(cv::CAP_PROP_FRAME_HEIGHT, 1080);
	while(!m_camera.grab())
		usleep(100);

	while(!m_camera.retrieve(m_image))
		usleep(100);
#else
	/**
	 * Testing for
	 * 2121		-> not focused
	 * 2121_2	-> not focused
	 * 226429_1
	 * 226429_2
	 * 226435_1
	 * 226435_2
	 * 226436_1
	 * 226436_2
	 * 226436_3
	 * 226439_1
	 * 226439_2
	 */
	m_path = "../images/new/" + getFile(curr_index);
	m_image = cv::imread(m_path, cv::IMREAD_COLOR);
	std::cout << m_path << "\n";
#endif
}

void Detector::orderCorners(cv::Mat &input, cv::Point2f* output)
{
	if (input.total() != 4)
		throw std::runtime_error("orderCorners input != 4 points");

	cv::Point avg(0,0);
	for (size_t i = 0; i < 4; i++)
	{
		avg += input.at<cv::Point>(i);
	}
	avg /= 4;
	for (size_t i = 0; i < 4; i++)
	{
		int x = input.at<cv::Point>(i).x;
		int y = input.at<cv::Point>(i).y;

		if (x < avg.x)
		{
			if (y < avg.y)
				output[0] = cv::Point(x,y);
			else
				output[1] = cv::Point(x,y);
		}
		else
		{
			if (y > avg.y)
				output[2] = cv::Point(x,y);
			else
				output[3] = cv::Point(x,y);
		}
	}
	
}

void Detector::fakeTransform()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;

	cv::Point2f src_vertices[4];

	switch (curr_index)
	{
	case 0:
		src_vertices[0] = cv::Point(520,160);
		src_vertices[1] = cv::Point(347, 1011);
		src_vertices[2] = cv::Point(1515, 1015);
		src_vertices[3] = cv::Point(1358, 155);
		break;

	case 1:
		src_vertices[0] = cv::Point(527, 193);
		src_vertices[1] = cv::Point(355, 1046);
		src_vertices[2] = cv::Point(1523, 1042);
		src_vertices[3] = cv::Point(1360, 186);
		break;

	case 2:
		src_vertices[0] = cv::Point(533, 179);
		src_vertices[1] = cv::Point(363, 1040);
		src_vertices[2] = cv::Point(1536, 1040);
		src_vertices[3] = cv::Point(1367, 186);
		break;

	case 3:
		src_vertices[0] = cv::Point(530, 163);
		src_vertices[1] = cv::Point(360, 1015);
		src_vertices[2] = cv::Point(1532, 1012);
		src_vertices[3] = cv::Point(1362, 163);
		break;


	default:
		throw std::runtime_error("Invalid index");
	}
	


	cv::Point2f dst_vertices[4];
	dst_vertices[0] = cv::Point(0, 0);
	dst_vertices[1] = cv::Point(0, 1190);
	dst_vertices[2] = cv::Point(1190, 1190);
	dst_vertices[3] = cv::Point(1190, 0);

	cv::Mat warpPerspectiveMatrix = getPerspectiveTransform(src_vertices, dst_vertices);
	warpPerspective(m_image, m_image, warpPerspectiveMatrix, cv::Size(1190, 1190), cv::INTER_LINEAR, cv::BORDER_CONSTANT);

	end = std::chrono::high_resolution_clock::now();
	std::cout << "TransformRifle: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " µs\n";

#if defined(DEBUG)
	cv::imshow("Transform result", m_image);
	cv::imwrite("transformed.png", m_image);
	cv::waitKey();
#endif
}
