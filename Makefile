# CXX=g++
# CXXFLAGS?=-Wall -Wmaybe-uninitialized -pedantic -Werror -g -O0 -std=c++17

CXX=g++
CXXFLAGS=-Wall -pedantic -Werror -g -O0 -std=c++17
OUTFILES=compress uncompress

# Detect if the compiler is Clang or GCC
ifeq ($(shell $(CXX) --version | head -n 1 | grep -c "clang"), 1)
    CXXFLAGS += -Wuninitialized
else
    CXXFLAGS += -Wmaybe-uninitialized
endif

all: $(OUTFILES)

compress: compress.cpp Helper.cpp HCTree.cpp Helper.hpp HCTree.hpp
	$(CXX) $(CXXFLAGS) -o compress compress.cpp Helper.cpp HCTree.cpp

uncompress: uncompress.cpp Helper.cpp HCTree.cpp Helper.hpp HCTree.hpp
	$(CXX) $(CXXFLAGS) -o uncompress uncompress.cpp Helper.cpp HCTree.cpp

gprof: compress.cpp uncompress.cpp Helper.cpp HCTree.cpp Helper.hpp HCTree.hpp
	make clean
	$(CXX) $(CXXFLAGS) -pg -o compress compress.cpp Helper.cpp HCTree.cpp
	$(CXX) $(CXXFLAGS) -pg -o uncompress uncompress.cpp Helper.cpp HCTree.cpp

clean:
	rm -f $(OUTFILES) *.o