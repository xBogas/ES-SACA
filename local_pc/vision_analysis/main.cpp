#include <opencv2/opencv.hpp>
#include "src/Detector.hpp"
#include "src/Approx.hpp"
#include <fstream>

int main(int argc, char const *argv[])
{
	int type;
	std::cin >> type;
	Detector det(type, 8888, "127.0.0.1");

	//det.testMain();
	/* std::cin.get();
	det.testMain(); */

	return 0;
}
