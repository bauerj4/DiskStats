#include "../include/Compile_Time_Options.h"
#include "../include/Globals.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <cstdlib>

/*
  General linear algebra routines go here.  
  The motivation for using our own is purely for OpenMP 
  parallelism and to dodge library dependencies for
  HPCVL.
*/


/*
  Return vector magnitude
*/

double Global::Magnitude(std::vector<double> &a){
  double aMag;

  aMag = 0;

  for (unsigned int i = 0; i < a.size(); i++){
    aMag += a[i] * a[i];
  }

  aMag = pow(aMag,0.5);
  return aMag;
}

/*
  Normalize vector
*/

void Global::Normalize(std::vector<double> &a){
  double aMag;

  aMag = 0;

  for (unsigned int i = 0; i < a.size(); i++){
    aMag += a[i] * a[i];
  }

  aMag = pow(aMag,0.5);

  for (unsigned int i = 0; i < a.size(); i++){
    a[i] /= aMag;
  }
}

/*
  Rotate into original system frame
*/

#ifdef ROTATE_SYSTEM
void Global::RotateToBoxFrame(std::vector<double> &x){
  Global::SqrMatrixVectorMultiply(Global::eulerMatrixT, x, x);
}

/*
  Rotate into disk frame
*/

void Global::RotateToDiskFrame(std::vector<double> &x){
  Global::SqrMatrixVectorMultiply(Global::eulerMatrix, x, x);
}
#endif

/*
  Generate Euler matrix from specified axis.
*/

void Global::EulerMatrix(std::vector<double> &a, std::vector<std::vector<double> > &E){
  double theta;// azimuth
  double phi;  // polar angle
  std::vector<double> axis(3,0);
  std::vector<std::vector<double> > R1(3, std::vector<double>(3,0)); //First rotation matrix
  std::vector<std::vector<double> > R2(3, std::vector<double>(3,0)); //Second rotation matrix
  std::vector<std::vector<double> > Euler(3, std::vector<double>(3,0));

  theta = -acos(a[2]);
  phi   = -atan2(a[1], a[0]);

  axis[0] = sin(theta) * cos(phi);
  axis[1] = sin(theta) * sin(phi);
  axis[2] = cos(theta);

  std::cout << "Euler matrix generated from axis: " << std::endl;
  std::cout << "[" << axis[0] << "," << axis[1] << "," << axis[2] << "]" << std::endl;
  std::cout << "with angles (phi =" << phi << ", theta = " << theta << ")" << std::endl; 

  R1[0][0] = cos(theta);
  R1[0][1] = 0.;
  R1[0][2] = -sin(theta);
  
  R1[1][0] = 0.;
  R1[1][1] = 1.;
  R1[1][2] = 0.;
 
  R1[2][0] = sin(theta);
  R1[2][1] = 0.;
  R1[2][2] = cos(theta);


  R2[0][0] = cos(phi);
  R2[0][1] = sin(phi);
  R2[0][2] = 0.;
  
  R2[1][0] = -sin(phi);
  R2[1][1] = cos(phi);
  R2[1][2] = 0.;

  R2[2][0] = 0.;
  R2[2][1] = 0.;
  R2[2][2] = 1.;

  Global::SqrMatrixMultiply(R2, R1, Euler);
  for (unsigned int i = 0; i < 3; i++)
    for (unsigned int j = 0; j < 3; j++)
      E[i][j] = Euler[j][i];
}

/*
  Sort the 3X3 eigenspace
*/

#ifdef ROTATE_SYSTEM
void Global::SortEigenspace(){
  std::vector<std::vector<double> > sortedSpace(4,std::vector<double>(3,0));
  double maxEigenvalue;
  int maxIndex;

  for (unsigned int k = 0; k < 3; k++){
    maxEigenvalue = 0.;
    maxIndex = -1;
    for (unsigned int i = 0; i < Global::eigenSpace[0].size(); i++){
      if (Global::eigenSpace[0][i] > maxEigenvalue){
	maxIndex = i;
	maxEigenvalue = Global::eigenSpace[0][i];
      }
    }
    
    //std::cout << "k = " << k << std::endl;
    sortedSpace[0][k] = maxEigenvalue;
    sortedSpace[1][k] = Global::eigenSpace[maxIndex + 1][0];
    sortedSpace[2][k] = Global::eigenSpace[maxIndex + 1][1];
    sortedSpace[3][k] = Global::eigenSpace[maxIndex + 1][2];

    Global::eigenSpace[0][maxIndex] = 0.;
    eigenSpace[maxIndex + 1][0] = 0.;
    eigenSpace[maxIndex + 1][1] = 0.;
    eigenSpace[maxIndex + 1][2] = 0.;
  }
  
  //delete Global::eigenSpace;
  for (unsigned int i = 0; i < Global::eigenSpace.size(); i++)
    for (unsigned int j = 0; j < Global::eigenSpace.size(); j++)
      eigenSpace[i][j] = sortedSpace[i][j];
}
#endif

/*
  Return the eigenvalues and eigenvectors as a
  N + 1 X N matrix.  The first row will contain the
  vector magnitudes of columns 1,2, ... N, and
  the last N columns will be the corresponding 
  normalised eigenvectors.  Use QR algorithm.
*/

void Global::Eigenspace(std::vector<std::vector<double> > &A,   std::vector<std::vector<double > > &E){

  std::vector<std::vector<double > > QT(A.size(),\
					std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > R(A.size(),\
				       std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > U(A.size(),\
                                       std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > lastQT(A.size(),\
                                        std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > lastR(A.size(),\
                                       std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > lastU(A.size(),\
                                       std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > NaNCheck(A.size(),\
					      std::vector<double>(A.size(), 0.));
  std::vector<std::vector<double > > result(A.size() + 1,\
					    std::vector<double>(A.size(), 0.));


  int converged;
  int hasNaNs;
  converged = 0;
  hasNaNs = 0;
  for (int j = 0; j < A.size(); j++){
    U[j][j] = 1;
  }


 
  /*
    Do a series of QR decompositions to converge to the
    eigenbasis

    while not converged do
      QR(A_k)

    Convergence criterion is KS statistic on each
    element (i.e. max difference in any element).  
  */


  //for (int i = 0; i < nIterations; i++){
  while (!converged){
    Global::HouseholderQR(A,QT,R);

    //Global::SqrTransposeArg2MatrixMultiply(R,QT,NaNCheck);
    for (unsigned int i = 0; i < NaNCheck.size(); i++){
      for (unsigned int j = 0; j < NaNCheck.size(); j++){
	if (U[i][j]  != U[i][j] ||\
	    QT[i][j] != QT[i][j] ||\
	    R[i][j]  != R[i][j])
	  hasNaNs = 1;
      }
    }

    if (hasNaNs){
      std::cout << "Encountered NaNs, unknown convergence status.  Continuing." << std::endl;
      for (unsigned int i = 0; i < NaNCheck.size(); i++){
	for (unsigned int j = 0; j < NaNCheck.size(); j++){

	  U[i][j] = lastU[i][j];
	  R[i][j] = lastR[i][j];
	  QT[i][j] = lastQT[i][j];
	}
      }
      break;
    }

    //Global::HouseholderQR(A,QT,R);


    Global::SqrTransposeArg2MatrixMultiply(R,QT,A);
    Global::SqrTransposeArg1MatrixMultiply(QT,U,U);

    Global::CheckConvergence(A,converged);

    for (unsigned int i = 0; i < NaNCheck.size(); i++){
      for (unsigned int j = 0; j < NaNCheck.size(); j++){
	lastQT[i][j] = QT[i][j];
	lastR[i][j] = R[i][j];
	lastU[i][j] = U[i][j];
      }
    }

  }

  
  
  std::cout << "Q transpose = " << std::endl;
  Global::PrintMatrix(QT);

  std::cout << "U = " << std::endl;
  Global::PrintMatrix(U);

  std::cout << "R = " << std::endl;
  Global::PrintMatrix(R);
  
  
  
  Global::SqrTransposeArg1MatrixMultiply(QT,R,A);
 
  /*
  
  std::cout << "A = " << std::endl;
  Global::PrintMatrix(A);
  
  */
  result[0][0] = A[0][0];
  result[0][1] = A[1][1];
  result[0][2] = A[2][2];

  result[1][0] = U[0][0];
  result[1][1] = U[1][0];
  result[1][2] = U[2][0];
  
  result[2][0] = U[0][1];
  result[2][1] = U[1][1];
  result[2][2] = U[2][1];
  
  result[3][0] = U[0][2];
  result[3][1] = U[1][2];
  result[3][2] = U[2][2];

  /*
  for (unsigned int i = 1; i <= A.size(); i++)
    for (unsigned int j = 0; j < A.size(); j++)
    result[i][j] = U[j][i - 1];*/
  //return result;

  for (unsigned int i = 0; i < Global::eigenSpace.size(); i++)
    for (unsigned int j = 0; j < Global::eigenSpace.size(); j++)
      E[i][j] = result[i][j];

}


/*
  Check QR iterations convergence
*/

void Global::CheckConvergence(  std::vector<std::vector<double > > &A, int &converged){
  /*
    Frobenius norm of lower off diagonal elements less
    than some threshold is convergence criterion.
  */

  double norm;
  norm = 0;
  for (int i = 0; i < A.size(); i++)
    for (int j = 0; j < i; j++)
      norm += A[i][j] * A[i][j];

  norm = pow(norm,0.5);
  if (norm < 1.e-2)
    converged = 1;
  
}


/*
  Do Householder reflections to get the orthogonalized
  matrix and eigenvalues. THIS IMPLEMENTATION IS NOT 
  MEANT TO BE NUMERICALLY STABLE FOR VERY LARGE MATRICES.
  DEFLATION IS NOT PERFORMED AND I MAKE NO GUARANTEES THAT
  NUMERICAL STABILITY WILL BE ACHIEVED.
*/


void Global::HouseholderQR(std::vector<std::vector<double> > &A,	\
			   std::vector<std::vector<double> > &QT,	\
			   std::vector<std::vector<double> > &R){

  // The Householder matrix
  std::vector<std::vector<double > > P(A.size(),\
				       std::vector<double>(A.size(), 0.));

  // The identity matrix
  std::vector<std::vector<double > > I(A.size(),\
				       std::vector<double>(A.size(), 0.));

  std::vector<double> v(A.size(), 0); // The hyperplane normal

  double alpha; // The signed component in undesirable directions
  double r; // normalization
  double vNorm;
  double d;

  /*
  std::cout << "Initial A =" << std::endl;
  Global::PrintMatrix(A);
  */
  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      if (i == j)
	QT[j][j] = 1;
      else
	QT[i][j] = 0;
    }
  }


  /*
    Set R = A
  */

  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      R[i][j] = A[i][j];
    }
  }
  /*
    Set identity matrix elements
  */

  for (int i = 0; i < A.size(); i++)
    I[i][i] = 1;
  
  /*
    Loop over all subdiagonal rows
  */

  //for (int k = 0; k < A.size() - 1; k++){
  for (int k = 0; k < A.size() - 1 ; k++){
    alpha = d = r = vNorm =  0;

    /*
      Set alpha and r
    */

  
    for (int j = k; j < A.size(); j++){
      alpha += A[j][k] * A[j][k];
    }

    alpha = pow(alpha, 0.5);
    r = (alpha * alpha - A[k+1][k] * alpha)/2.; 
    r = pow(r,0.5);
    
    /*
      Initialize the Householder vector
    */

    for (int i = 0; i < A.size(); i++){
      if (i < k)
        v[i] = 0;  
      else if (i == k){
	v[i] = pow((alpha - A[k][k])/(2. * alpha), 0.5);
	v[i] *= -alpha / fabs(alpha);
      }
      else
	v[i] = -A[i][k] / (2. * alpha * v[k]);

      vNorm += v[i] * v[i];
    }

    //vNorm = pow(vNorm, 0.5);

    P = Global::Outer(v,v);
    Global::SqrMatrixScalarMultiply(P, 1./vNorm, P);
    Global::SqrMatrixScalarMultiply(P, 2., P);
    Global::SqrMatrixSubtract(I, P, P);

    Global::SqrMatrixMultiply(P,A,A);
    Global::SqrMatrixMultiply(P,QT,QT);
    Global::ForceZeros(A);

    /*
    std::cout << "P = " << std::endl;
    Global::PrintMatrix(QT);

    std::cout << "QT = " << std::endl;
    Global::PrintMatrix(QT);
    */
  }

  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      R[i][j] = A[i][j];
    }
  }

  Global::SqrTransposeArg1MatrixMultiply(QT,R,A);
  //SqrMatrixMultiply(QT,R,A);

  
  /*
  std::cout << "Q transpose = " << std::endl;
  Global::PrintMatrix(QT);
  
  std::cout << "R = " << std::endl;
  Global::PrintMatrix(R);
  
  //std::cout << "A = " << std::endl;
  //PrintMatrix(A);
  */
}


/*
  Force near zero elements to be zero
*/


void Global::ForceZeros(std::vector<std::vector<double> > &A){
  for (int i = 0; i < A[0].size(); i++){
    for (int j = 0; j < A.size(); j++){
      if (fabs(A[i][j]) < 1.e-10 )
	A[i][j] = 0.;
    }
  }
}

/*
  Matrix-vector multiply
*/

void Global::SqrMatrixVectorMultiply(std::vector<std::vector<double> > &A,    \
				     std::vector<double> &b,		\
				     std::vector<double> &c){

  std::vector<double> cTemp(A.size(),0);
  for (unsigned int i = 0; i < A.size(); i++)
    cTemp[i] = 0;

  for (unsigned int i = 0; i < A.size(); i++){
    for (unsigned int j = 0; j < A.size(); j++){
      cTemp[i] += A[i][j] * b[j];
    }
  }

  for (unsigned int i = 0; i < A.size(); i++)
    c[i] = cTemp[i];
}


/*
  Square matrix add
*/


void Global::SqrMatrixAdd(std::vector<std::vector<double> > &A,	\
			  std::vector<std::vector<double> > &B,	\
			  std::vector<std::vector<double> > &C){
  
  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);
  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = A[i][j] + B[i][j];
    }
  }
  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}


/*
  Square matrix subtract
*/


void Global::SqrMatrixSubtract(std::vector<std::vector<double> > &A,	\
			       std::vector<std::vector<double> > &B,    \
			       std::vector<std::vector<double> > &C){

  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);
  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = A[i][j] - B[i][j];
    }
  }
  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}  



/*
  Square matrix multiply
*/

void Global::SqrMatrixMultiply(std::vector<std::vector<double> > &A,	\
			       std::vector<std::vector<double> > &B,	\
			       std::vector<std::vector<double> > &C){
  
  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);

  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = 0.;
      for (int k = 0; k < A.size(); k++){
	cTemp[i][j] += A[i][k]*B[k][j];
      }
    }
  }

  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}

/*
  Square matrix multiply transposing
  the first matrix
*/

void Global::SqrTransposeArg1MatrixMultiply(std::vector<std::vector<double> > &A, \
					    std::vector<std::vector<double> > &B, \
					    std::vector<std::vector<double> > &C){

  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);

  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = 0.;
      for (int k = 0; k < A.size(); k++){
        cTemp[i][j] += A[k][i]*B[k][j];
      }
    }
  }

  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}


/*
  Square matrix multiply transposing
  the second matrix
*/

void Global::SqrTransposeArg2MatrixMultiply(std::vector<std::vector<double> > &A, \
					    std::vector<std::vector<double> > &B, \
					    std::vector<std::vector<double> > &C){

  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);

  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = 0.;
      for (int k = 0; k < A.size(); k++){
        cTemp[i][j] += A[i][k]*B[j][k];
      }
    }
  }

  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}

/*
  Scalar multiply a matrix
*/


void Global::SqrMatrixScalarMultiply(std::vector<std::vector<double> > &A, double b, \
				     std::vector<std::vector<double> > &C){
  std::vector<std::vector<double> > cTemp;
  for (int i = 0; i < C.size(); i++)
    cTemp.push_back(C[i]);
  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A.size(); j++){
      cTemp[i][j] = b * A[i][j];
    }
  }

  for (int i = 0; i < C.size(); i++)
    for(int j = 0; j < C.size(); j++)
      C[i][j] = cTemp[i][j];
}

/*
  Return the inner product of two vectors
*/

double Global::Dot(std::vector<double> &a, std::vector<double> &b){
  double dot = 0;
  for (unsigned int i = 0; i < a.size(); i++){
    dot += a[i] * b[i];
  }
  return dot;
}


/*
  Return the outer product of two vectors
*/

std::vector<std::vector<double> > Global::Outer(std::vector<double> &a,	\
						std::vector<double> &b){
  std::vector<std::vector<double > > out(a.size(),\
					 std::vector<double>(b.size(),0));
  for (int i = 0; i < a.size(); i++){
    for (int j = 0; j < b.size(); j++){
      out[i][j] = a[i] * b[j];
    }
  }
  //std::cout << "Finished outer product." << std::endl;

  return out;
}

/*
  This function acts similarly to NumPy's linspace,
  but it returns an array of length n+1 (containing
  both boundaries a and b).
*/

std::vector<double> Global::LinearSpacing(double a, double b, int n){
  double dx;
  double width;
  std::vector<double> result;

  if (b < a){
    std::cout << "Error in LinearSpacing: b < a" << std::endl;
    exit(3);
  }
  else if (n == 0){
    std::cout << "Error in LinearSpaceing: Divide by 0" << std::endl;
    exit(4);
  }

  width = b - a;
  dx = width/n;
  
  for (int i = 0; i < n; i++){
    result.push_back(a + i*dx);
  }

  result.push_back(b);

  return result;

}


/*
  Print matrix
*/

void Global::PrintMatrix(std::vector<std::vector<double> > &A){
  //std::cout << "A = " << std::endl;
  for (int i = 0; i < A.size(); i++){
    for (int j = 0; j < A[0].size(); j++){
      if (j == 0)
	std::cout << "[";

      std::cout << A[i][j] << ",";

      if (j == A[0].size() -1)
	std::cout << "]\n";
    }
  }
}
