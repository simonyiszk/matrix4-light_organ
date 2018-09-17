#include "matrix_animation.hpp"
#include <iostream>

using namespace std::chrono;

int main(){
	Frame f(100ms);

	f.getPixel(0,0).set(255,0,0);
	f.getPixel(0,1).set(0,255,0);
	f.getPixel(0,2).set(0,0,255);

	std::cout << f;

	return 0;
}
