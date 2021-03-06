#include <math.h>
#include <eigen3/Eigen/Dense>
#include <eigen3/Eigen/Cholesky>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "gameSetup.h"
#include <boost/math/distributions/normal.hpp>



using namespace std;

/// Set values of infinity and -infinity:
double ninf  = -numeric_limits<double>::infinity();
double inf  = numeric_limits<double>::infinity();

/// TODO: Evaluate Cholesky decomp, Find Error.

void genzAlgorithm(VectorXd a, VectorXd b, MatrixXd covar) 
{
  int N, m;
  
  double delta = 0.;
  double error = 1000.;
  double sum = 0.; 
  double varsum, itsum;
  
  N = 0; varsum = 0.; itsum = 0.; m = 3; 
  
  VectorXd d(m);
  VectorXd e(m); 
  VectorXd f(m);
  VectorXd y(m); 
  VectorXd w(m);
  
  MatrixXd C(3,3); 
  
  boost::math::normal norm;
  
  C << 1.0 , 0., 0. ,
  3./5., 4./5., 0.,
  1./3., 2./3.,2./3.;
 
  if(a[0] == ninf) d[0] = 0.;
  else d[0] = cdf(norm,(a[0]/C(1,1)));
  cout << "d0:" << d[0] << endl; 
  if(b[0] == inf) e[0] = 1.; 
  else e[0] = cdf(norm,(b[0]/C(1,1)));
  cout << "e0:" << e[0] << endl; 
  f[0] = e[0]-d[0]; 
  cout << "f0:" << e[0] << endl; 
  while(error>0.0001){
       /// Generate random sample:
      for(int k = 0; k<m;k++){
	w[k] = ((double) rand() / (RAND_MAX));
      }
      cout << "w:" << endl;
      cout << w << endl;
    for(int i = 1; i<m; i++){

      sum = 0.; 
      
      /// Evaluate Quantile Function:
      y[i-1] = quantile(norm,(d[i-1] + w[i-1]*(e[i-1]-d[i-1]))); 
      cout << "yi-1: " << y[i-1] << endl;
      /// Compute sum for evaluation of d and e:
      for(int j = 0; j<= i-1;j++){
	sum += C(i,j)*y[j];
      }
      cout << "sum " << sum << endl;
      /// Set d:
      if(a[i] == ninf) d[i] = 0.;
      else d[i] = cdf(norm,((a[i] -sum)/C(i,i))); 
      cout << "di: " << d[i] << endl;
      /// Set e:
      if(b[i] == inf) e[i] = 1.; 
      else e[i] = cdf(norm,((b[i] -sum)/C(i,i)));
      cout << "ei: " << e[i] << endl;
      /// Set f:
      f[i] = (e[i] - d[i])*f[i-1];
      cout << "fi: " << f[i] << endl;
    }
  
    N = N+1;

    /// Set delta:
    delta = (f[m-1] - itsum)/double(N);
    cout << "delta: " << delta << endl;
    /// Set itsum, varsum and delta:
    itsum = itsum + delta;
    cout << "itsum: " << itsum << endl;
    varsum = double(N-2)*varsum/double(N) + delta*delta;
    cout << "varsum: " << varsum << endl;
    error = 2.5*sqrt(varsum);
    cout << "intsum: " << itsum << endl;
    cout << "error: " << error << endl;
  }
}
int main(void){
  VectorXd a(3);a << ninf,ninf,ninf;
  VectorXd b(3);b << 1,4,2;
  MatrixXd covar(3,3); covar <<  1.0, 3./5.,       1./3.,
				 5./3.,  1.,      11./15.,
				  1./3.      ,  11./15.      ,       1.; 
  genzAlgorithm(a,b,covar);

}
