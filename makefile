all: subvector all_lib 

all_lib:
	g++ src/demo.cpp -Iinclude -o appMain --std=c++20 -g

subvector:
	g++ src/demo_subvector.cpp -Iinclude -o appSubvector --std=c++14 -g