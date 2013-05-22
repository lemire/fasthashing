#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h


CXXFLAGS = -g0 -ftree-vectorize -march=native -O3  -Wall  -Woverloaded-virtual  -Wsign-promo -Wold-style-cast  -mssse3 -pipe

VECTOR =  -ftree-vectorize 


#-DNDEBUG

all:  
	$(CXX) $(CXXFLAGS)     speedtesting.cpp   -o speedtesting

vector:
	$(CXX) $(CXXFLAGS)  $(VECTOR)   speedtesting.cpp   -o speedtesting

stupid:
	$(CXX) -O2 speedtesting.cpp   -o speedtesting



package:
	zip -9 faststronlyuniversalhashing_`date +%Y-%m-%d`.zip makefile README example.cpp hashfunctions.h ZRandom.h speedtesting.cpp ztimer.h

clean:
	rm -f *.o speedtesting
