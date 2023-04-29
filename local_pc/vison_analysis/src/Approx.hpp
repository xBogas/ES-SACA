#ifndef VISION_ANALYSIS_INCLUDE_APPROX_HPP
#define VISION_ANALYSIS_INCLUDE_APPROX_HPP

#include <iostream>
#include <cmath>
#include <array>

#include <opencv2/opencv.hpp>
#include <eigen3/Eigen/Dense>

// TODO: check for div error if Type = int
template<typename Type = double>
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
		append(const Type t)
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
		u << 550, 550; //starting point
	}

	Approx(Type x_init, Type y_init, Type radius)
		: m_iter(m_points), m_radius(radius)
	{
		u << x_init, y_init;
	}

	~Approx(){};

	// Operator for insertion into m_points matrix
	Approx&
	operator<<(Type t)
	{
		m_iter.append(t);
		return *this;
	}

	// Operator for insertion into m_points matrix
	Approx&
	operator,(Type t)
	{
		m_iter.append(t);
		return *this;
	}

	
	// Solve least squares 
	void
	nextIter()
	{
		h = jac.colPivHouseholderQr().solve(f);
		//jac. ldlt().solve(f)
				
		u.coeffRef(0) = u.coeff(0) + h.coeff(0);
		u.coeffRef(1) = u.coeff(1) + h.coeff(1);
	};

	void
	updateJac()
	{
		Type u1 = u.coeff(0);
		Type u2 = u.coeff(1);

		for (size_t i = 0; i < size(); i++)
		{
			Type x1 = m_points.coeff(i,0);
			Type x2 = m_points.coeff(i,1);
			
			Type dis = norm(u1, u2, x1, x2);
			jac.coeffRef(i,0) = (u1 - x1)/dis;
			jac.coeffRef(i,1) = (u2 - x2)/dis;
		}
	}

	void
	updateF()
	{
		Type u1 = u.coeff(0);
		Type u2 = u.coeff(1);
		
		for (size_t i = 0; i < size(); i++)
		{
			Type x1 = m_points.coeff(i,0);
			Type x2 = m_points.coeff(i,1);
			
			f.coeffRef(i) = -1 *(norm(u1, u2, x1, x2) - m_radius);
		}
	}

	void
	print()
	{
		std::cout << "Printing points:\n" << m_points << std::endl;
		std::cout << "Print jac:\n" << jac << std::endl;
		std::cout << "Print f:\n" << f << std::endl;
		std::cout << "Print u:\n" << u << std::endl;
	};

	double
	hNorm()
	{ return sqrt(pow(h.coeff(0),2)+pow(h.coeff(1),2)); }

	int
	size()
	{ return m_points.rows(); }

	void
	setInitialPoint(Type x, Type y)
	{
		u.coeffRef(0) = x;
		u.coeffRef(1) = y;
	}

	void
	setRadius(Type r)
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
		std::cout << "Here is matrix resized \n" << m_points << "\n";
	}

	std::tuple<Type,Type>
	getCenter()
	{
		return std::make_tuple(u.coeff(0), u.coeff(1));
	}

private:

	inline double
	norm(Type u1, Type u2, Type x1, Type x2)
	{ return sqrt(pow(u1-x1,2)+pow(u2-x2,2)); }

	typedef Eigen::Matrix<Type,Eigen::Dynamic,2> matrixD_t;
	InsertIter<matrixD_t> m_iter;
	Eigen::Matrix<Type,Eigen::Dynamic,2> m_points;
	Eigen::Matrix<Type,Eigen::Dynamic,2> jac;
	Eigen::Matrix<Type,Eigen::Dynamic,1> f;
	Eigen::Matrix<Type,2,1> u; 
	Eigen::Matrix<Type,2,1> h;
	Type m_radius;
};


#endif
