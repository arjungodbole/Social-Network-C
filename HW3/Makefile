output: user.o network.o social_network.o post.o
	g++ -std=c++17 user.o network.o social_network.o post.o -o output
user.o: user.cpp user.h
	g++ -std=c++17 -c user.cpp -o user.o
network.o: network.h user.h network.cpp
	g++ -std=c++17 -c network.cpp -o network.o
social_network.o: network.h user.h social_network.cpp
	g++ -std=c++17 -c social_network.cpp -o social_network.o
post.o: post.cpp post.h
	g++ -std=c++17 -c post.cpp -o post.o




