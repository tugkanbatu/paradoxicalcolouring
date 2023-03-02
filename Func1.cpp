class Func{
public:
  //  Func(long double key, long double *vec);
  virtual long double operator()(long double x)=0;
  long double k;
  virtual long double innerfn(long double x)=0;
  
private:
  //long double *v;
  //  long double *numerator, *denom, *series;
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



// Func class implementation

Func_iter::Func_iter(long double key, long double *vec){
  k=key;
  v=vec;
}

  
long double Func_iter::innerfn(long double x){
  static int minindex=maxC+1,maxindex=0;
  if (x>=1)
    return 1e+10;
  //  if (x<1/100.0)
  //  return 0;
  int i = (int) floor( M * x);
  //  if (i==1)
  //  cout << "v_1(1) is accessed" << endl;
  // if (i<minindex){
  //   cout << endl << "Minimum index used into v_1 is : " << i << endl;
  //   minindex=i;
  // }
  // if (i>maxindex){
  //   cout << endl << "Maximum index used into v_1 is : " << i << endl;
  //   maxindex=i;
  // }
  // if (i<minC)
  //   i=minC;
  // if (i>maxC)
  //   i=maxC;
  return v[i];
}

long double Func_iter::operator()(long double x){
  return innerfn((1.0-x) / k) / x;
}
 

// Func class implementation

Func_ini::Func_ini(long double key){
  k=key;

  series = new long double[4];
  series[0] = 1.0;
  series[1] = 5.0/3072.0;
  //  series[2] = 1112.0/17.0/45.0/1024.0/1024.0;
  series[2] = 100.0/17.0/9.0/1024.0/1024.0;
  series[3] = 125.0/1024.0/1024.0/1024.0/4096.0;
}

  
long double Func_ini::innerfn(long double x){

  //  cout << x << endl;
  
  long double result=series[3];
  for (int i=2;i>=0;i--){
    result *= x;
    result += series[i];
  }
  //  cout << result << endl;

  
  result *= (1+5.0*x) * (1+5.0*x/16.0) * (1+5.0*x/256);
  // cout << result << endl;
  result /= (1-5.0*x/4.0) * (1-5.0*x/64.0) * (1-5.0*x/1024.0);
  // cout << result << endl;
  return result;
}

long double Func_ini::operator()(long double x){
  return innerfn((1.0-x)/k-0.2) / x;
} 


