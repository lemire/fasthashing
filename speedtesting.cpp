/**
* You may consider this code to be in the public domain.
* 
* Daniel Lemire, September 2010.
*
*/
/**
* Compile as g++ -O3 -o speedtesting speedtesting.cpp
*/
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <typeinfo>
#include "ztimer.h"
#include "hashfunctions.h"
#include "ZRandom.h"

using namespace std;


template <class T, class INTEGER>
double testSpeed(const T & hasher, const vector<INTEGER> & data, uint64 & answer, const uint mN) {
	ZTimer t;
	double timeelapsed;
	t.reset();
	answer += hasher.hash(&data[0],&data[0]+mN);
	timeelapsed = t.split()/(1000.0);
	return timeelapsed;
}

template <class T, class INTEGER>
double testSpeedManyTimes(const T & hasher, const vector<INTEGER> & data, uint64 & answer, const uint mN,const uint times) {
	ZTimer t;
	double timeelapsed;
	t.reset();
    for(uint k = 0; k<times;++k)
	answer += hasher.hash(&data[0],&data[0]+mN);
	timeelapsed = t.split()/(1000.0);
	return timeelapsed;
}

int main(int params, char ** args) {
	uint N;
	if (params >= 2) 
	  N = atoi(args[1]);
	else {
	  N = 1024*1024;
	}
    cout<<"#sizeof(uint64)="<<sizeof(uint64)<<endl;
    assert(sizeof(uint64)==8);
	cout<<"# Initializing data..."<<endl;
	vector<uint32>data(N);
	vector<uint64>randombuffer64(N+1);
	//MTRand mt;
	ZRandom zr;
	for(uint k = 0;k<N;++k) {
		data[k] =  zr.getValue();//mt.randInt();
	}
	data.push_back(1);// so that it never ends with a zero
	if( (data.size() & 1) != 0) data.push_back(0);// make sure it is even
	cout<<"# done generating data"<<endl;
	cout<<"# "<<data.size()*sizeof(uint)/(1024.0*1024.0)<<" MB"<<endl;
	ZTimer t;
	for(uint k = 0;k<=N+2;++k) {
		randombuffer64[k] =zr.getValue() & (static_cast<uint64>(zr.getValue())<<32 ) ;
	}
	double timeelapsed = t.split()/(1000.0);
	cout<<"# random generated in "<<timeelapsed<< " or "<<N*sizeof(uint64)/(1024.0*1024*1024*timeelapsed)<<" GB/s"<<endl;
	cout<<"# "<<data.size()*sizeof(uint64)/(1024.0*1024.0)<<" MB"<<endl;
	const uint times = 20;
	uint64 answer = 0;
  	Silly silly;
	Thorup thorup(randombuffer64);
  	StrongMultilinear sm(randombuffer64);
  	PyramidalMultilinear pm(randombuffer64);
  	XAMA xama(randombuffer64);
#if defined (__PCLMUL__) && (__SSE2__)
    CLStrongMultilinear clsm(randombuffer64); 
#endif
  	NoMultiplication testing(randombuffer64);
  	StrongMultilinearTwoByTwo sm2by2(randombuffer64);
    const uint shorttimes =2000000;
	cout<<"# Starting tests... repeating each run "<<shorttimes<<" times"<<endl;
	cout<<"# N silly thorup09(not-strongly-universal) xama strong-multilinear strong-multilinear-2by2 pyramidalmultilinear clmulti*"<<endl;
	for(uint mN = 1024; mN<=2048; mN*=2) {
		vector<double> counter(6,0); 
  	  	counter[0]+=testSpeedManyTimes(silly,data,answer,mN,shorttimes);
	  	counter[1]+=testSpeedManyTimes(thorup,data,answer,mN,shorttimes);
	  	counter[2]+=testSpeedManyTimes(xama,data,answer,mN,shorttimes);
	  	counter[3]+=testSpeedManyTimes(sm,data,answer,mN,shorttimes);
	  	counter[4]+=testSpeedManyTimes(sm2by2,data,answer,mN,shorttimes);
	  	counter[5]+=testSpeedManyTimes(pm,data,answer,mN,shorttimes);
#if defined (__PCLMUL__) && (__SSE2__)
	  	counter[6]+=testSpeedManyTimes(clsm,data,answer,mN,shorttimes);
#endif
        cout<<mN<<" ";
        for(uint k = 0; k<counter.size();++k)
            cout<<counter[k]<<" ";
        cout<<endl;
	}
    cout<<endl;
	cout<<"# Starting tests... repeating each run "<<times<<" times"<<endl;
	cout<<"# N silly thorup09(not-strongly-universal) xama strong-multilinear strong-multilinear-2by2 pyramidalmultilinear clmulti*"<<endl;
	for(uint mN = 1048576; mN<=data.size(); mN*=2) {
		vector<double> counter(6,0); 
		for(uint k = 0;k<times;++k) {
  	  		counter[0]+=testSpeed(silly,data,answer,mN);
	  		counter[1]+=testSpeed(thorup,data,answer,mN);
	  	    counter[2]+=testSpeedManyTimes(xama,data,answer,mN,shorttimes);
	  		counter[3]+=testSpeed(sm,data,answer,mN);
	  		counter[4]+=testSpeed(sm2by2,data,answer,mN);
	  		counter[5]+=testSpeed(pm,data,answer,mN);
#if defined (__PCLMUL__) && (__SSE2__)
	  	counter[6]+=testSpeedManyTimes(clsm,data,answer,mN,shorttimes);
#endif
		}
        cout<<mN<<" ";
        for(uint k = 0; k<counter.size();++k)
            cout<<counter[k]<<" ";
        cout<<endl;
	}
    cout<<endl;

	return answer;	
}

