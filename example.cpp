#include "hashfunctions.h"
#include "ZRandom.h"


// compile me with gcc : g++ -o example example.cpp 
int main(int params, char ** args) {
	unsigned int some32bitstring[3] = {32321,32323,32132132}; // string should not end with zero, append a 1 if you must
	// construct a random buffer, you can do this once
	uint N = 128; // limit on the length of strings
	vector<uint64>randombuffer64(N+2);
	ZRandom zr;
	for(uint k = 0;k<=N+2;++k) 
		randombuffer64[k] =zr.getValue() & (static_cast<uint64>(zr.getValue())<<32 ) ;
	// we construct the hash function
	StrongMultilinear sm(randombuffer64);
	unsigned int * startpointer = &(some32bitstring[0]);
	unsigned int * endpointer = startpointer+3;
	uint hashvalue = sm.hash(startpointer,endpointer);
}