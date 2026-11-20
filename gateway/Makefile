CXX?=g++
CXXFLAGS=-std=c++17 -O2 -Wall -Wextra
all: gateway test_gateway
gateway: main.cpp gateway.cpp gateway.hpp
	$(CXX) $(CXXFLAGS) -o $@ main.cpp gateway.cpp
test_gateway: test_gateway.cpp gateway.cpp gateway.hpp
	$(CXX) $(CXXFLAGS) -o $@ test_gateway.cpp gateway.cpp
test: test_gateway
	./test_gateway
clean:
	rm -f gateway test_gateway
