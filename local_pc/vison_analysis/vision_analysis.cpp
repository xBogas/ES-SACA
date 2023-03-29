#include "vision_analysis.h"

Detector::Detector(QObject *parent): QObject(parent), m_camara(0)
{
	if (!m_camara.isOpened())
	{
		std::cerr << "ERROR: Could not open camara\n";
		exit(EXIT_FAILURE);
	}

	if(!m_camara.read(m_image))
		std::cerr << "Failed to grab camara frame\n";

}

Detector::~Detector()
{
	
}