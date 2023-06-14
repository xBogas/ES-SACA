#ifndef VISION_ANALYSIS_INCLUDE_APPROX_HPP
#define VISION_ANALYSIS_INCLUDE_APPROX_HPP

#include <iostream>
#include <cmath>
#include <array>
#include <fstream>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

class Approx
{
private:
	
	template<typename T>
	struct InsertIter
	{
		T& m_ref;
		int i_rows;
		int i_col;

		InsertIter(T& ref)
			: m_ref(ref), i_rows(0), i_col(0)
		{}

		void
		append(const double t)
		{
			if (i_col == m_ref.cols())
			{
				i_rows++;
				i_col = 0;
			}
			if (i_rows >= m_ref.rows())
			{
				std::cerr << "Out of bounds insertion of points\n";
				exit(EXIT_FAILURE);
			}
			m_ref.coeffRef(i_rows, i_col++) = t;
		}
	};
public:

	Approx()
		: m_iter(m_points)
	{
		u << 525, 525; //starting point
	}

	Approx(double x_init, double y_init)
		: m_iter(m_points)
	{
		u << x_init, y_init;
	}

	~Approx(){};

	// Operator for insertion into m_points matrix
	Approx&
	operator<<(double t)
	{
		m_iter.append(t);
		return *this;
	}

	// Operator for insertion into m_points matrix
	Approx&
	operator,(double t)
	{
		m_iter.append(t);
		return *this;
	}

	
	// Solve least squares 
	void
	nextIter()
	{
		// jac*h = f
		h = jac.colPivHouseholderQr().solve(f);
		// h = jac.fullPivHouseholderQr().solve(f); //! 596.73-591.78

		// h = (jac.transpose() * jac).ldlt().solve(jac.transpose() * f);

		u.coeffRef(0) = u.coeff(0) + h.coeff(0);
		u.coeffRef(1) = u.coeff(1) + h.coeff(1);
	};

	void
	updateJac()
	{
		double u1 = u.coeff(0);
		double u2 = u.coeff(1);

		for (size_t i = 0; i < size(); i++)
		{
			double x1 = m_points.coeff(i,0);
			double x2 = m_points.coeff(i,1);
			
			double dis = norm(u1, u2, x1, x2);
			jac.coeffRef(i,0) = (u1 - x1)/dis;
			jac.coeffRef(i,1) = (u2 - x2)/dis;
#ifdef DEBUG
			/* if (jac.coeffRef(i,0) == 0 || jac.coeffRef(i,1) == 0)
				throw std::runtime_error("Jac = 0"); */
#endif	
		}
	}

	void
	updateF()
	{
		double u1 = u.coeff(0);
		double u2 = u.coeff(1);
		
		for (size_t i = 0; i < size(); i++)
		{
			double x1 = m_points.coeff(i,0);
			double x2 = m_points.coeff(i,1);
			
			f.coeffRef(i) = -1 *(norm(u1, u2, x1, x2) - m_radius);
		}
	}

	void
	print()
	{
		std::fstream file;
		file.open("log.txt", std::ios::out);
		/* file << "Printing points:\n" << m_points << std::endl;
		file << "Print jac:\n" << jac << std::endl;
		file << "Print f:\n" << f << std::endl;
		file << "Print u:\n" << u << std::endl; */
		file << m_points;
		file.close();
	};

	double
	hNorm()
	{ return sqrt(pow(h.coeff(0),2)+pow(h.coeff(1),2)); }

	int
	size()
	{ return m_points.rows(); }

	void
	setInitialPoint(double x, double y)
	{
		u.coeffRef(0) = x;
		u.coeffRef(1) = y;
	}

	void
	setRadius(double r)
	{
		m_radius = r;
	}

	void
	insertPoints(const std::vector<cv::Point>& data)
	{
		m_points.resize(data.size(), 2);
		jac.resize(data.size(), 2);
		f.resize(data.size(),1);

		for (size_t i = 0; i < data.size(); i++)
		{
			m_points.coeffRef(i,0) = data[i].x;
			m_points.coeffRef(i,1) = data[i].y;
		}
	}

	std::tuple<double,double>
	getCenter()
	{
		return std::make_tuple(u.coeff(0), u.coeff(1));
	}

private:

	inline double
	norm(double u1, double u2, double x1, double x2)
	{ return sqrt(pow(u1-x1,2)+pow(u2-x2,2)); }

	typedef Eigen::Matrix<double,Eigen::Dynamic,2> matrixD_t;
	InsertIter<matrixD_t> m_iter;
	Eigen::Matrix<double,Eigen::Dynamic,2> m_points;
	Eigen::Matrix<double,Eigen::Dynamic,2> jac;
	Eigen::Matrix<double,Eigen::Dynamic,1> f;
	Eigen::Matrix<double,2,1> u; 
	Eigen::Matrix<double,2,1> h;
	double m_radius;
};


#endif
