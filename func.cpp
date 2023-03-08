// Implementation of w

long double wseries[4];
long double wvec[maxC+1];

// Function w
long double w(long double x){
  long double xshifted = x - 0.2L;
  long double result=wseries[3];
  for (int i=2;i>=0;i--){
    result *= xshifted;
    result += wseries[i];
  }
  result *= (1+5.0L*xshifted) * (1+5.0L*xshifted/16.0L) * (1+5.0L*xshifted/256.0L);
  result /= (1-5.0L*xshifted/4.0L) * (1-5.0L*xshifted/64.0L) * (1-5.0L*xshifted/1024.0L);

  // Adjustments for the bottom and top end of the range x in (0,1)
  long double g = 0.0L; // adjustment factor
  if (x<0.2L){
    g = 0.2L - x;
    g *= -(20.0L/27.0L)*g*g;
  }
  if (x>0.5L){
    g = x - 0.5L;
    g *= -0.25L*g*g;
  }
  g = exp(g);
  result *= g;
  return result;
}

// coefficient array for truncated series in w is initialised
// wvec is populated with w() values for discretised points in (0,1)
void wsetup(){
  wseries[0] = 1.0L;
  wseries[1] = 5.0L/3072.0L;
  wseries[2] = 100.0L/17.0L/9.0L/1024.0L/1024.0L;
  wseries[3] = 125.0L/1024.0L/1024.0L/1024.0L/4096.0L;

  for(int m=minC;m<=maxC;m++)
    wvec[m] = w(m/M);
}


// Implementation of the w((1-x)/k)/x
class Func{
public:
  Func(int k);
  long double operator()(long double x);
  
private:
  int k;
};

Func::Func(int key){
  k=key;
}
  
long double Func::operator()(long double x){
  long double xshifted=(1.0L-x)/k;
  if (xshifted>=1.0)
    cout << "x>=1 !!!" << endl;
  int i = (int) floor( M * xshifted);
  return wvec[i]/x;
}

 


