// Implementation of w

long double wseries[4];

// coefficient array for truncated series in w
void wsetup(){
  wseries[0] = 1.0L;
  wseries[1] = 5.0L/3072.0L;
  wseries[2] = 100.0L/17.0L/9.0L/1024.0L/1024.0L;
  wseries[3] = 125.0L/1024.0L/1024.0L/1024.0L/4096.0L;
}

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
  long double g; // adjustment factor
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




/////////////////////////////////////////////////////////////////



class Func{
public:
  virtual long double operator()(long double x)=0;
  long double k;
  virtual long double innerfn(long double x)=0;
};

class Func_iter:public Func{
public:
  Func_iter(long double key, long double *vec);
  long double operator()(long double x);
  long double innerfn(long double x);
  
private:
  long double *v;
};



Func_iter::Func_iter(long double key, long double *vec){
  k=key;
  v=vec;
}
  
long double Func_iter::innerfn(long double x){
  static int minindex=maxC+1,maxindex=0;
  if (x>=1)
    return 1e+10;
  int i = (int) floor( M * x);
  return v[i];
}

long double Func_iter::operator()(long double x){
  return innerfn((1.0-x) / k) / x;
}
 


