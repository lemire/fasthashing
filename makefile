#
.SUFFIXES:
#
.SUFFIXES: .cpp .o .c .h

# set your architecture by, e.g., export myarch="-march=nocona"

CXXFLAGS = -g0 -ftree-vectorize -O3  -Wall  -Woverloaded-virtual  -Wsign-promo -Wold-style-cast  -mssse3 -pipe

VECTOR =  -ftree-vectorize 


#-DNDEBUG

all:  
	g++ $(CXXFLAGS) ${myarch}    speedtesting.cpp   -o speedtesting

vector:
	g++ $(CXXFLAGS) ${myarch} $(VECTOR)   speedtesting.cpp   -o speedtesting

stupid:
	g++ -O2 speedtesting.cpp   -o speedtesting



package:
	zip -9 faststronlyuniversalhashing_`date +%Y-%m-%d`.zip makefile README example.cpp hashfunctions.h ZRandom.h speedtesting.cpp ztimer.h

clean:
	rm -f *.o speedtesting
