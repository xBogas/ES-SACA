#include "Detector.hpp"
#ifdef RELEASE
Detector::Detector(QObject *parent): QObject(parent), m_camara(0)
{

}
#else
Detector::Detector(QObject *parent): QObject(parent)
{
	std::cout << "I don't have a camera\n"
				<< "Running with test images\n";

}
#endif