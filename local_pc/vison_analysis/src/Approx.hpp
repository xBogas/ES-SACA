#ifndef VISION_ANALYSIS_INCLUDE_APPROX_HPP
#define VISION_ANALYSIS_INCLUDE_APPROX_HPP

#include <iostream>
#include <cmath>
#include <array>

#include <eigen3/Eigen/Dense>

// TODO: check for div error if Type = int
template<int __M_size, typename Type = double>
class Approx
{
private:
	typedef Eigen::Matrix<Type,__M_size,2> __M_Iter_t;
	
	struct InsertIter
	{
		__M_Iter_t& m_ref;
		int i_rows;
		int i_col;

		InsertIter(__M_Iter_t& ref)
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
		u << 5.3794, 7.2532; //starting point
	}

	Approx(Type x_init, Type y_init, Type radius)
		: m_iter(m_points), m_radius(radius)
	{
		u << x_init, y_init;
	}

	~Approx(){};

	// Overload for insertion into m_points matrix
	Approx&
	operator<<(Type t)
	{
		m_iter.append(t);
		return *this;
	}

	// Overload for insertion into m_points matrix
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
	{ return __M_size; }

private:

	inline double
	norm(Type u1, Type u2, Type x1, Type x2)
	{ return sqrt(pow(u1-x1,2)+pow(u2-x2,2)); }

	InsertIter m_iter;
	Eigen::Matrix<Type,__M_size,2> m_points;
	Eigen::Matrix<Type,__M_size,2> jac;
	Eigen::Matrix<Type,__M_size,1> f;
	Eigen::Matrix<Type,2,1> u; 
	Eigen::Matrix<Type,2,1> h;
	Type m_radius;
};


#endif
