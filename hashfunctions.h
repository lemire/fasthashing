/**
* You may consider this code to be in the public domain.
* 
* Daniel Lemire, September 2010.
*
*/

#ifndef HASHFUNCTION
#define HASHFUNCTION

typedef unsigned int uint;
typedef unsigned long long uint64;
typedef unsigned int uint32;

#include <vector>
#include <cassert>

using namespace std;

/**
* Inspired by the Java hashCode method for strings.
*/
class Silly {
	public:

	  Silly() {
	  }
	  
	  uint32 hash(const uint32 * p, const uint32  * const endp)  const {
	  	uint32 sum = 0;
	  	for(;p!=endp;++p) {
	  		sum = 37 * sum + *p;
	  	}
	  	return sum;
	  }
};



/*
* This is just like StrongMultilinear but with a bit of loop unrolling. Strongly universal.
* Assumes an even number of elements. 
* Also assumes that the string does not exceed the size of the provided randombuffer (no checking).
*/
class StrongMultilinear {
	public:

	  StrongMultilinear(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }
	  
	  template <class INTEGER>
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomdata = & firstfew[0];
	  	uint64 sum = *(randomdata++);
	  	for(;p!=endp;++p,++randomdata) {
	  		sum+= *randomdata * static_cast<uint64>(*p);
	  	}
	  	return static_cast<uint32>(sum>>32);
	  }

	  const vector<uint64> & firstfew;
};

/*
* This is just like StrongMultilinear but with a bit of loop unrolling. Strongly universal.
* Assumes an even number of elements. 
* Also assumes that the string does not exceed the size of the provided randombuffer (no checking).
*/
class StrongMultilinearTwoByTwo {
	public:

	  StrongMultilinearTwoByTwo(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }
	  
	  template <class INTEGER>
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomdata = & firstfew[0];
	  	uint64 sum = *(randomdata++);
	  	for(;p!=endp;p+=2,randomdata+=2) {
	  		sum+= *randomdata * static_cast<uint64>(*p) + *(randomdata + 1) * static_cast<uint64>(*( p + 1 ));
	  	}
	  	return static_cast<uint32>(sum>>32);
	  }

	  const vector<uint64> & firstfew;
};


/*
* From Thorup, SODA 09. Strongly universal.
* Assumes an even number of elements.
* Also assumes that the string does not exceed the size of the provided randombuffer (no checking).
*/ 
class Thorup {
	public :
	  Thorup(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }

	  template <class INTEGER>	  
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomdata = & firstfew[0];
	  	uint64 sum = 0;
	  	for(;p!=endp;p+=2,randomdata+=2) {
	  		sum ^= (*randomdata  + *p) * (*(randomdata+1)  + *(p+1));
	  	}
	  	return static_cast<uint32>(sum>>32);
	  }

	  const vector<uint64> & firstfew;

};



#endif
