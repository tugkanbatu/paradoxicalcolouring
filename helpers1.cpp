
// implements function (1-(1-x/2)^{2n-1})^{n-1} - x
// to find the value of r using bisection solver rSolver
long double rfun(long double x, long double n){

  double result = pow((1.0-(x/2.0)),2*n-1);
  result = 1 - result;
  result = pow(result,n-1)-x;
  return result;
}

// bisection solver for rfun() function
long double rSolver(long double n,long double target,long double lb,long double ub,long double accuracy){
  long double mid;
  do{
    mid=(lb+ub)/2;
    //    cout << mid << "   "; 
    long double y=rfun(mid,n);
    // cout << y << endl;
    if (y>=target)
      lb=mid;
    else
      ub=mid;
    }while (ub-lb >= accuracy);
  return mid;
}

// factorial function for n>=1
int factorial(int n){
  int f = 1;
  while (n>1)
    f *= n--;
  return f;
}

// calculates no. of different permutations of j1,j2,j2,j4
// 1 <= j1 <= j2 <= j3 <= j4 <= maxtypes
int noperm(int j1, int j2, int j3, int j4){
  int t[4];
  t[0]=j1;
  t[1]=j2;
  t[2]=j3;
  t[3]=j4;

  int no = 24; // 4!
  int i = 1; // index 
  int rep = 1; // number of repetitions of a repeated value
  while(i<4){
    if (t[i]==t[i-1]){
      i++;
      rep++;
    }
    else{
      no /= factorial(rep);
      i++;
      rep=1;
    }
  }
  no /= factorial(rep);
  return no;
}

int *binomcoef;

void calcbinomcoef(const int n)
{
  binomcoef[0]=1;
  for(int i=1;i<=n;i++){
    binomcoef[i] = binomcoef[i-1] * (n-i+1) / i;
  }
}
  
long double probfn(int i,int j,int k,int l,long double r){ 
  long double result=1.0;
  result *= binomcoef[i];
  result *= binomcoef[j];
  result *= binomcoef[k];
  result *= binomcoef[l];
  result *= pow(1.0*r/2.0,i+j+k+l);
  result *= pow((1.0-1.0*r/2.0),36-i-j-k-l);  // 36 = NoFns * types

  return result;
}
  

// BisectionSolver tries to solve for x in [lb,ub] such that f(x)=target
// It stops when ub-lb < accuracy
// It initially assumes and maintains that f(lb) >= target >= f(ub)
long double BisectionSolver(Func* f,long double B,long double target,long double lb,long double ub,long double accuracy){
  long double mid;
  do{
    mid=(lb+ub)/2;
    long double y=(*f)(mid);
    if (y>=target)
      lb=mid;
    else
      ub=mid;
    }while (ub-lb >= accuracy);
  return mid;
}

// SimulSolver tries to solve nofn functions simultaneously such that
// fn[1](x_1)=fn[2](x_2)=...=fn[nofn](x_nofn) and x_1+x_2+...+x_nofn=B
// The common function value should be in [lb,ub]
// fn[i]s are assumed to be non-increasing
// The equalities are checked within accuracy
// SimulSolver updates argument array roots with corresponding x_i values
void SimulSolver(Func* fn[],int nofn,long double* roots, long double B,long double lb,long double ub,long double accuracy){
  do{
    long double sum=0.0;
    long double mid=(lb+ub)/2;
    for(int i=0;i<nofn;i++){
      roots[i]=BisectionSolver(fn[i],B,mid,0.0,1.0,accuracy); 
      sum+=roots[i];
    }
    if (B-sum >= accuracy)
      ub=mid;
    else if (sum-B>accuracy)
      lb=mid;
    else
      break;
  }while (ub-lb >= accuracy);
  return;
}
