#include <opencv2/opencv.hpp>
#include "src/Detector.hpp"
#include "src/Approx.hpp"
#include <fstream>

int main(int argc, char const *argv[])
{

	Detector det;

	return 0;
/* 	//load data points
	std::ifstream file;
	file.open("../Data.txt");
	if (!file.is_open())
	{
		std::cerr << "Failed to load data file\n";
		exit(1);
	}
	
	std::string line;
	Approx<1'000,double> demo(0,0, 10);

	while (getline(file, line))
	{
		double x,y;
		int pos = line.find(';');
		x = std::stod(line.substr(0,pos));
		y = std::stod(line.substr(pos+1,std::string::npos));

		demo << x,y;
	}
	
	double h_norm = 1;
	double min = 1E-3;

	while (h_norm > min)
	{
		demo.updateJac();
		demo.updateF();
		demo.nextIter();
		//demo.print();
		h_norm = demo.hNorm();
	}
	
	demo.print();


	return 0; */
}
