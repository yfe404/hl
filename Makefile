CXXFLAGS = -Wall -Werror -fopenmp -O3 -I./hl
LIB=hl/*.hpp
PROGRAMS= hhl akiba degree lcheck ghl benchmark

all: $(PROGRAMS)

$(PROGRAMS):%: %.cpp $(LIB) Makefile
	$(CXX) $(CXXFLAGS) -o $@ $<

clean:
	rm $(PROGRAMS)
