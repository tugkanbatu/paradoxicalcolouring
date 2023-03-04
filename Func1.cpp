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

class Func_ini:public Func{
public:
  Func_ini(long double key);
  long double operator()(long double x);
  long double innerfn(long double x);
  
private:
  long double *numerator, *denom, *series;
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
 
Func_ini::Func_ini(long double key){
  k=key;
  series = new long double[4];
  series[0] = 1.0;
  series[1] = 5.0/3072.0;
  series[2] = 100.0/17.0/9.0/1024.0/1024.0;
  series[3] = 125.0/1024.0/1024.0/1024.0/4096.0;
}

long double Func_ini::innerfn(long double x){
  long double result=series[3];
  for (int i=2;i>=0;i--){
    result *= x;
    result += series[i];
  }
  result *= (1+5.0*x) * (1+5.0*x/16.0) * (1+5.0*x/256);
  result /= (1-5.0*x/4.0) * (1-5.0*x/64.0) * (1-5.0*x/1024.0);
  return result;
}

long double Func_ini::operator()(long double x){
  return innerfn((1.0-x)/k-0.2) / x;
} 


