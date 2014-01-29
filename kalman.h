#ifndef KALMAN_H
#define KALMAN_H

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/lu.hpp>
#include <math.h>

#include "position.h"
#include "tracked_object.h"

namespace uBlast = boost::numeric::ublas;
typedef uBlast::matrix<double> mat;

class kalman
{

public:
	kalman();
	// kalman(mat x, mat P, mat K, mat R, mat z);
	// kalman(mat A, mat Q, mat H, mat S1, mat S2);
	// kalman(mat A, mat Q, mat H, mat S1, mat S2, mat x, mat P, mat K, mat R, mat z);
	~kalman();
	position getNewPosition(position pos1, position pos2);

private:
	const uBlast::matrix<double> A, Q, H, S1, S2;
	uBlast::matrix<double> x, P, K, R, z;
	const uBlast::matrix<double> getA();
	const uBlast::matrix<double> getH();
	bool InvertMatrix(const uBlast::matrix<double>& input, uBlast::matrix<double>& inverse);
	uBlast::matrix<double> transform(uBlast::matrix<double> X1, double d, double theta, double alpha, double fi);
	uBlast::matrix<double> covariance(double d, double theta, double alpha, double fi, uBlast::matrix<double> S1);
	uBlast::matrix<double> fuzeKovariance(uBlast::matrix<double> S1, uBlast::matrix<double> S2);
	uBlast::matrix<double> fuzeSouradnic(uBlast::matrix<double> X1,uBlast::matrix<double> S1,uBlast::matrix<double> X2,uBlast::matrix<double> S2);
	
};


#endif // KALMAN_H