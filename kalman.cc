#include "kalman.h"

	kalman::kalman()
		:	A(getA()),
			Q(0.025 * uBlast::identity_matrix<double>(6)),
			H(getH()),
			S1(4 * uBlast::identity_matrix<double>(7)),
			S2(36 * uBlast::identity_matrix<double>(7)),
			x(uBlast::scalar_matrix<double>(6, 1)), 
			P(uBlast::identity_matrix<double>(6)), 
			K(uBlast::matrix<double>(6, 6)), 
			z(uBlast::matrix<double>(3, 1))		
	{

	}

	kalman::kalman(mat x_, mat P_, mat K_, mat z_)
		:	x(x_), P(P_), K(K_), z(z_),
			A(getA()),
			Q(0.025 * uBlast::identity_matrix<double>(6)),
			H(getH()),
			S1(4 * uBlast::identity_matrix<double>(7)),
			S2(36 * uBlast::identity_matrix<double>(7))
	{

	}

	kalman::kalman(mat A_, mat Q_, mat H_, mat S1_, mat S2_)
		:	A(A_), Q(Q_), H(H_), S1(S1_), S2(S2_), 
			x(uBlast::scalar_matrix<double>(6, 1)), 
			P(uBlast::identity_matrix<double>(6)), 
			K(uBlast::matrix<double>(6, 6)), 
			z(uBlast::matrix<double>(3, 1))
	{

	}

	kalman::kalman(mat A_, mat Q_, mat H_, mat S1_, mat S2_, mat x_, mat P_, mat K_, mat z_)
		:	A(A_), Q(Q_), H(H_), S1(S1_), S2(S2_), x(x_), P(P_), K(K_), z(z_)
	{

	}

	kalman::~kalman()
	{

	}

	const uBlast::matrix<double> kalman::getA()
	{	
		uBlast::matrix<double> A (6, 6);
		for(unsigned i = 0; i < A.size1 (); ++ i)
		{
			A(i, i) = 1;
		}
		A(0, 1) = 1;
		A(2, 3) = 1;
		A(4, 5) = 1;

		return A;
	}


	const uBlast::matrix<double>  kalman::getH()
	{	
		uBlast::matrix<double> H (3, 6);
		H(0, 0) = 1;
		H(1, 2) = 1;
		H(2, 4) = 1;

		return H;
	}

	bool InvertMatrix(const uBlast::matrix<double>& input, uBlast::matrix<double>& inverse)
	{
		using namespace uBlast;
		typedef permutation_matrix<std::size_t> pmatrix;

		// create a working copy of the input
		matrix<double> A(input);

		// create a permutation matrix for the LU-factorization
		pmatrix pm(A.size1());

		// perform LU-factorization
		int res = lu_factorize(A, pm);
		if (res != 0)
			return false;

		// create identity matrix of "inverse"
		inverse.assign(identity_matrix<double> (A.size1()));

		// backsubstitute to get the inverse
		lu_substitute(A, pm, inverse);

		return true;
	}

	position kalman::getNewPosition(position pos1, position pos2)
	{
		uBlast::matrix<double> aX1, aX2, aS1, aS2, R, inv, tmpProd;
		double d1, d2, theta1, theta2, alpha1, alpha2, fi1, fi2;
		uBlast::matrix<double> xx1 (3, 1);
		xx1(1,0) = pos1.x;
		xx1(2,0) = pos1.y;
		xx1(3,0) = pos1.z;

		uBlast::matrix<double> xx2 (3, 1);
		xx2(1,0) = pos2.x;
		xx2(2,0) = pos2.y;
		xx2(3,0) = pos2.z;

		d1 = sqrt(pow(pos1.x,2) + pow(pos1.y,2) + pow(pos1.z,2));
		fi1 = atan2(-1 * pos1.z, pos1.x);
		alpha1 = asin(pos1.y/d1);
		theta1 = 0;

		d2 = sqrt(pow(pos2.x,2) + pow(pos2.y,2) + pow(pos2.z,2));
		fi2 = atan2(-1 * pos2.z, pos2.x);
		alpha2 = asin(pos2.y/d2);
		theta2 = 0;

		/* ALGORITMUS!!!!!!!!!!!! */
		//Prepocteni systemu
		aX1 = transform(xx1, d1, theta1, alpha1, fi1);
		aX2 = transform(xx2, d2, theta2, alpha2, fi2);

		aS1=covariance(d1, theta1, alpha1, fi1, S1);
		aS2=covariance(d2, theta2, alpha2, fi2, S2);

		//Fuze souradnic
		z = fuzeSouradnic(aX1, aS1, aX2, aS2);
		R = fuzeKovariance(aS1,aS2);

		//Kalman
		x = prod(A, x);
		tmpProd = prod(P, trans(A));
		P = prod(A, tmpProd + Q);

		tmpProd = prod(P, trans(H));
		tmpProd = prod(H, tmpProd);
		InvertMatrix(tmpProd + R,inv);

		tmpProd = prod(trans(H), inv);
		K = prod(P, tmpProd);

		tmpProd = prod(H, x);
		x = x + prod(K, (z - tmpProd));

		tmpProd = prod(H, P);
		P = P - prod(K,tmpProd);
	}

	uBlast::matrix<double> kalman::transform(mat X1, double d, double theta, double alpha, double fi)
	{
		uBlast::matrix<double> X2 (3, 1);
		X2(0, 0) = X1(0, 0) + d*cos(alpha)*cos(fi + theta);
		X2(1, 0) = X1(1, 0) + d*cos(alpha)*sin(fi + theta);
		X2(2, 0) = X1(2, 0) + d*sin(alpha);

		return X2;

	}

	uBlast::matrix<double> kalman::covariance(double d, double theta, double alpha, double fi, mat S1)
	{
		uBlast::matrix<double> H (3, 7);
		uBlast::matrix<double> tmpProd;

		H(0, 0) = 1;
		H(1, 1) = 1;
		H(2, 2) = 1;
		H(0, 3) = -d*cos(alpha)*sin(fi + theta);
		H(0, 4) = cos(alpha)*cos(fi + theta);
		H(0, 5) = -d*cos(alpha)*sin(fi + theta);
		H(0, 6) = -d*sin(alpha)*cos(fi + theta);
		
		H(1, 3) = d*cos(alpha)*cos(fi + theta);
		H(1, 4) = cos(alpha)*sin(fi + theta);
		H(1, 5) = d*cos(alpha)*cos(fi + theta);
		H(1, 6) = -d*sin(alpha)*sin(fi + theta);
		
		H(2, 4) = sin(alpha); 
		H(2, 6) = -d*cos(alpha);

		//H*S1*H'
		tmpProd = prod(S1, trans(H));
   		return prod(H, tmpProd);

	}

	uBlast::matrix<double> kalman::fuzeKovariance(uBlast::matrix<double> S1, uBlast::matrix<double> S2)
	{	
		uBlast::matrix<double> inv (S1.size2(), S1.size1());
		uBlast::matrix<double> tmpProd;

		InvertMatrix(S1+S2, inv);

		//S1*inv(S1+S2)*S2
		tmpProd = prod(inv, S2);
		return prod(S1, tmpProd);
	}

	uBlast::matrix<double> kalman::fuzeSouradnic(uBlast::matrix<double> X1,uBlast::matrix<double> S1,uBlast::matrix<double> X2,uBlast::matrix<double> S2)
	{
		uBlast::matrix<double> inv (S1.size2(), S1.size1());
		uBlast::matrix<double> tmpProd;

		InvertMatrix(S1+S2, inv);

    	//X2 + S2*inv(S1+S2)*(X1-X2)
    	tmpProd = prod(S2, inv);
    	return X2 + prod(tmpProd, (X1 - X2));
	}