// Compile command:
// g++ genanalysis_threaded.cpp -o genericanalysis -pthread -std=c++11

#include<iostream>
#include<fstream>
#include<cmath>
#include<thread>

using namespace std;

const long double accuracy = 1e-11L;
const long double lb=0.0L,ub=1e+8L; // Bounds for the function values
const int N=5;        //
const int NoFns=N-1;  // no of functions
const int types=9;
const int jconfigs = 495; // #(a,b,c,d)\in types^4 s.t. a<=b<=c<=d

const int maxC=19999;
const long double M = (long double) maxC+1;
const int minC=1;

long double vcur[maxC+1];
// long double vnext[maxC+1];

long double r;

long double maxdiff1[types+1][types+1][types+1][types+1];
long double maxjumpval[types+1][types+1][types+1][types+1];
long double mintail[types+1][types+1][types+1][types+1];
long double mingen[types+1][types+1][types+1][types+1];

#include "Func1.cpp"
#include "helpers1.cpp"

void generic_thread(int j1, int j2, int j3, int j4){

  Func* Fns[NoFns];
  long double roots[NoFns];
  Fns[0] = new Func_iter(j1,vcur);
  Fns[1] = new Func_iter(j2,vcur);
  Fns[2] = new Func_iter(j3,vcur);
  Fns[3] = new Func_iter(j4,vcur);

  long double mxd=0.0L, mxj=0.0, mnt=1.0L;
  mingen[j1][j2][j3][j4]=ub;
    
  for(int m=1;m<=maxC;m++){
    long double budget = 1.0 - (m/M);

    SimulSolver(Fns,NoFns,roots,budget,lb,ub,accuracy);

    if (j1==j2-1)
      mxd = max(mxd,abs(roots[1]-roots[0]));
    if (j2==j3-1)
      mxd = max(mxd,abs(roots[2]-roots[1]));
    if (j3==j4-1)
      mxd = max(mxd,abs(roots[3]-roots[2]));
    if (j1<j2)
      mxj = max(mxj,roots[1]);
    if (j1<j3)
      mxj = max(mxj,roots[2]);
    if (j1<j4)
      mxj = max(mxj,roots[3]);
    if (j2<j3)
      mxj = max(mxj,roots[2]);
    if (j2<j4)
      mxj = max(mxj,roots[3]);
    if (j3<j4)
      mxj = max(mxj,roots[3]);
    if (j1>1 && j2>j1)
      mnt = min(mnt, (m/M) + roots[2] + roots[3]);

    long double t = (*Fns[0])(roots[0]);
    long double ratio = log(m*t/M) - log(vcur[m]);
    if (ratio < mingen[j1][j2][j3][j4])
      mingen[j1][j2][j3][j4] = ratio;
    
  }
  maxdiff1[j1][j2][j3][j4] = mxd;
  maxjumpval[j1][j2][j3][j4] = mxj;
  mintail[j1][j2][j3][j4] =  mnt;
}

int main(){
  cout.precision(5);
  
  binomcoef = new int[types+1];
  calcbinomcoef(types);

  r=rSolver(N,0.0,0.9,1.0,accuracy); // r should be very close 1
  cout << "The value of \\hat{q} from Lemma 2: " << r << endl;

  // for calculating v_1 from innerfn()
  Func_ini f(1);
  // Adjustments for the bottom and top end of the range
  for(int m=minC;m<=maxC;m++){
    vcur[m]=f.innerfn(m/M - 1.0/N);
    //    cout << m << " : " << vcur[m] << endl;
    long double g = 1.0;
    if (m<4000){
      g = (m/M - 0.2);
      g *= (20.0/27.0)*g*g;
      g = exp(g);
    }
    if (m>10000){
      g = (m/M - 0.5);
      g *= -0.25*g*g;
      g = exp(g);
    }
    vcur[m] *= g;
  }

  thread *myth[jconfigs];
  int t_i = 0;
  for(int j1=1;j1<=types;j1++){
    for(int j2=j1;j2<=types;j2++){
      for(int j3=j2;j3<=types;j3++){
	for(int j4=j3;j4<=types;j4++){
	  myth[t_i] = new thread(generic_thread,j1,j2,j3,j4);
	  t_i++;
	}
      }
    }
  }

  t_i = 0;
  for(int j1=1;j1<=types;j1++){
    for(int j2=j1;j2<=types;j2++){
      for(int j3=j2;j3<=types;j3++){
	for(int j4=j3;j4<=types;j4++){
	  if ((*myth[t_i]).joinable()){
	    (*myth[t_i]).join();
	    t_i++;
	  }
	}
      }
    }
  }

  long double mxd=0.0L, mxj=0.0L, mnt=1.0L;
  long double sum = 0.0L;
  for(int j1=1;j1<=types;j1++){
    for(int j2=j1;j2<=types;j2++){
      for(int j3=j2;j3<=types;j3++){
	for(int j4=j3;j4<=types;j4++){
	  mxd = max(mxd,maxdiff1[j1][j2][j3][j4]);
	  mxj = max(mxj,maxjumpval[j1][j2][j3][j4]);
	  mnt = min(mnt,mintail[j1][j2][j3][j4]);

	  long double p = probfn(j1,j2,j3,j4,r);
	  long double w = mingen[j1][j2][j3][j4];
	  sum += w*p*noperm(j1,j2,j3,j4);
	}
      }
    }
  }

  cout << endl << "Proof of Lemma 5." << endl;
  cout << "The maximum difference |q_k - q_l| for j_k=j_l+1: " << mxd << endl;
  cout << "The maximum q_k for j_k>j_l: " << mxj << endl;
  cout << "The minimum value of p+q_3+q_4 for j_2>j_1>1: " << mnt << endl << endl;
  
  sum /= r;
  cout << "Proof of Lemma 7." << endl;
  cout << "The expectation of the ratios: " << sum << endl;
}  

