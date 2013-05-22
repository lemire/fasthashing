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
* Basic Strongly universal method.
* Also assumes that the string does not exceed the size of the provided randombuffer (no checking).
*/
class StrongMultilinear {
	public:

	  StrongMultilinear(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }
	  
	  template <class INTEGER>
#if defined(__GNUC__) && !( defined(__clang__) || defined(__INTEL_COMPILER  ))
__attribute__((optimize("no-tree-vectorize")))
// GCC has buggy SSE2 code generation in some cases
// Thanks to Nathan Kurz for noticing that GCC 4.7 requires no-tree-vectorize to produce correct results.
#endif
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
#if defined (__PCLMUL__) && (__SSE2__)

#include <wmmintrin.h>
uint32 barrettWithoutPrecomputation( __m128i A) {
        const uint64 irredpoly = 1UL+(1UL<<2)+(1UL<<6)+(1UL<<7)+(1UL<<32);
        const int n = 32;// degree of the polynomial
        const __m128i C = _mm_set_epi64x(0,irredpoly);// C is the irreducible poly.
        __m128i Q1 = _mm_srli_epi64 (A, n);
        __m128i Q2 = _mm_clmulepi64_si128( Q1, C, 0x00);// A div x^n
        __m128i Q3 = _mm_srli_epi64 (Q2, n);
        __m128i final  = _mm_xor_si128 (A, _mm_clmulepi64_si128( Q3, C, 0x00));
        return _mm_cvtsi128_si64(final);
}

/**
* fast multilinear hashing based on carry-less multiplication.
* Limitation: assumes that length is a multiplie of 4. No handling of special cases.
*/
class CLStrongMultilinear {
	public:

	  CLStrongMultilinear(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }
	  
	  template <class INTEGER>
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomsource = & firstfew[0];
        assert((endp - p) / 4 * 4 == (endp - p));
        const uint32 *  randomsource32 = ( const uint32 * )randomsource;
        __m128i acc = _mm_set_epi64x(0,*(randomsource32));
        randomsource32 += 4;
        const __m128i zero =  _mm_setzero_si128 ();
        for(; p!= endp; randomsource32+=4,p+=4 ) {
          const __m128i temp1 = _mm_load_si128((__m128i * )randomsource32);
          const __m128i temp2 = _mm_load_si128((__m128i *) p);
          const __m128i twosums = _mm_xor_si128(temp1,temp2);
          const __m128i part1 = _mm_unpacklo_epi32(twosums,zero);
          const __m128i clprod1  = _mm_clmulepi64_si128( part1, part1, 0x10);
          acc = _mm_xor_si128 (clprod1,acc);
          const __m128i part2 = _mm_unpackhi_epi32(twosums,zero);
          const __m128i clprod2  = _mm_clmulepi64_si128( part2, part2, 0x10);
          acc = _mm_xor_si128 (clprod2,acc);
       }
       return barrettWithoutPrecomputation(acc);
      }

	  const vector<uint64> & firstfew;
};


#endif


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
#if defined(__GNUC__) && !( defined(__clang__) || defined(__INTEL_COMPILER  ))
__attribute__((optimize("no-tree-vectorize")))
// GCC has buggy SSE2 code generation in some cases
// Thanks to Nathan Kurz for noticing that GCC 4.7 requires no-tree-vectorize to produce correct results.
#endif
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
* From Thorup, SODA 09. Almost universal. **Not strongly universal**
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

/**
* A bit like what Thorup proposed in SODA 09, but actually strongly universal.
* Assumes an even number of elements.
* Also assumes that the string does not exceed the size of the provided randombuffer (no checking).
*/
class XAMA {
	public :
	  XAMA(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }

	  template <class INTEGER>	  
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomdata = & firstfew[0];
	  	uint64 sum = *(randomdata++);
	  	for(;p!=endp;p+=2,randomdata+=2) {
	  		sum += (*randomdata  + *p) * (*(randomdata+1)  + *(p+1));
	  	}
	  	return static_cast<uint32>(sum>>32);
	  }

	  const vector<uint64> & firstfew;

};
// just for testing purposes
class NoMultiplication {
	public:

	  NoMultiplication(const vector<uint64> & randombuffer) : firstfew(randombuffer) {
	  }
	  
	  template <class INTEGER>
	  uint32 hash(const INTEGER * p, const INTEGER  * const endp)  const {
	  	const uint64 * randomdata = & firstfew[0];
	  	uint64 sum = *(randomdata++);
	  	for(;p!=endp;++p,++randomdata) {
            sum+= (*p)^(*randomdata) ;//*randomdata + static_cast<uint64>(*p);
	  	}
	  	return static_cast<uint32>(sum>>32);
	  }

	  const vector<uint64> & firstfew;
};
#endif
