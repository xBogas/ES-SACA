#include <opencv2/opencv.hpp>
#include "src/Detector.hpp"
#include "src/Approx.hpp"
#include <fstream>

int main(int argc, char const *argv[])
{
	/* int type;
	std::cin >> type; */
	Detector det(0, 8888, "127.0.0.1");

	
	for(int i = 0; i < 4 ; i++)
	{
		det.testMain(false);
		det.testMain(true);
	}

	return 0;
}
