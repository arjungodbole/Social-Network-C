output: user.o network.o social_network.o
	g++ -std=c++17 user.o network.o social_network.o -o output
user.o: user.cpp user.h
	g++ -std=c++17 user.cpp -o user.o
network.o: network.h user.h network.cpp
	g++ -std=c++17 network.cpp -o network.o
social_network.o: network.h user.h social_network.cpp
	g++ -std=c++17 social_network.cpp -o social_network.o



