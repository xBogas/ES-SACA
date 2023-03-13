#include <iostream>
#include <cmath>
#include <array>

#include <eigen3/Eigen/Dense>

#define RADIUS	4.1088

/*
template<typename Mprt>
class InsertValue
{
public:
	template<typename Scalar>
	InsertValue(Mprt* t, Scalar arg) 
		: m_prt(t), i_rows(0), i_col(0), m_pos(1)
	{
		m_prt->coeffRef(0,0) = arg;
	};

	template<typename Scalar>
	InsertValue& operator,(Scalar args)
	{
		if (i_col == m_prt->cols())
		{
			i_rows++;
			i_col = 0;
		}		
		return *this;
	}
	~InsertValue(){};

private:

	Mprt* m_prt;
	int i_rows;
	int i_col;
	int m_pos;
};
*/ 

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

		void append(const Type t)
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

	InsertIter iter;
	Eigen::Matrix<Type,__M_size,2> points;
	Eigen::Matrix<Type,__M_size,2> jac;
	Eigen::Matrix<Type,__M_size,1> f;
	Eigen::Matrix<Type,2,1> u; 
	Eigen::Matrix<Type,2,1> h;

	int size()
	{ return __M_size; }
	
	inline double 
	norm(Type u1, Type u2, Type x1, Type x2)
	{
		return sqrt(pow(u1-x1,2)+pow(u2-x2,2));
	}

	
	
public:

	void nextIter()
	{
		//std::cout << "The least-squares solution is:\n" << h << "\n";

		h = jac.colPivHouseholderQr().solve(f);
		//jac. ldlt().solve(f)
		
		std::cout << "The least-squares solution is:\n" << h << "\n";
		
		u.coeffRef(0) = u.coeff(0) + h.coeff(0);
		u.coeffRef(1) = u.coeff(1) + h.coeff(1);
	}

	void updateJac()
	{
		Type u1 = u.coeff(0);
		Type u2 = u.coeff(1);

		for (size_t i = 0; i < size(); i++)
		{
			Type x1 = points.coeff(i,0);
			Type x2 = points.coeff(i,1);
			
			Type dis = norm(u1, u2, x1, x2);
			jac.coeffRef(i,0) = (u1 - x1)/dis;
			jac.coeffRef(i,1) = (u2 - x2)/dis;
		}
	}

	void updateF()
	{
		Type u1 = u.coeff(0);
		Type u2 = u.coeff(1);
		
		for (size_t i = 0; i < size(); i++)
		{
			Type x1 = points.coeff(i,0);
			Type x2 = points.coeff(i,1);
			
			f.coeffRef(i) = -1 *(norm(u1, u2, x1, x2) - RADIUS);
		}
	}

	void print()
	{
		std::cout << "Printing points:\n" << points << std::endl;
		std::cout << "Print jac:\n" << jac << std::endl;
		std::cout << "Print f:\n" << f << std::endl;
		std::cout << "Print u:\n" << u << std::endl;
	};

	double hNorm()
	{
		return sqrt(pow(h.coeff(0),2)+pow(h.coeff(1),2));
	}


	double err()
	{
		double sum = 0;

		return 1;
	}


	Approx() : iter(points)
	{
		u << 5.3794, 7.2532;//starting point
	};
	~Approx(){};
	
	/*
	template<typename T>
	InsertValue<__M_Iter_t> operator<<(const T& t)
	{
		return InsertValue<__M_Iter_t>(&points, t);
	}
	*/

	Approx& operator<<(Type t)
	{
		iter.append(t);
		return *this;
	}

	Approx& operator,(Type t)
	{
		iter.append(t);
		return *this;
	}
};

int main(int argc, char const *argv[])
{
	const int samples = 6;

	Approx<samples> m;
	m << 1,7,
		 2,6,
		 5,8,
		 7,7,
		 9,5,
		 3,7;
	
	double norm_h = 10;
	double min = 1E-6;
	
	while (norm_h > min)
	{
		m.updateJac();
		m.updateF();
		m.nextIter();
		m.print();
		norm_h = m.hNorm();
	}

	return 0;
}
