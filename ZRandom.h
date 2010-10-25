#ifndef __ZRANDOM_H__
#define __ZRANDOM_H__



//
//  Mersenne twister - random number generator.
//  Generate uniform distribution of 32 bit integers with the MT19937 algorithm.
// source: http://bannister.us/weblog/?s=Mersenne

class ZRandom
{

private:
    enum { N = 624, M = 397 };
    unsigned MT[N+1];
    unsigned* map[N];
    int nValues;

public:
    ZRandom(unsigned iSeed = 20070102);
    unsigned getValue();

};

ZRandom::ZRandom(unsigned iSeed) : nValues(0)
{
    // Seed the array used in random number generation.
    MT[0] = iSeed;
    for (int i=1; i<N ; ++i) {
        MT[i] = 1 + (69069 * MT[i-1]);
    }
    // Compute map once to avoid % in inner loop.
    for (int i=0; i<N; ++i) {
        map[i] = MT + ((i + M) % N);
    }
}

unsigned ZRandom::getValue()
{
    if (0 == nValues) {
        MT[N] = MT[0];
        for (int i=0; i<N ; ++i) {
            register unsigned y = (0x80000000 & MT[i]) | (0x7FFFFFFF & MT[i+1]);
            register unsigned v = *(map[i]) ^ (y >> 1);
            if (1 & y) v ^= 2567483615;
            MT[i] = v;
        }
        nValues = N;
    }
    register unsigned y = MT[N - nValues--];
    y ^= y >> 11;
    y ^= (y << 7) & 2636928640;
    y ^= (y << 15) & 4022730752;
    y ^= y >> 18;
    return y;
}

#endif