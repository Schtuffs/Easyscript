#include <iostream>
#include "Easyscript.hpp"

std::ostream& operator<<(std::ostream& cout, Pixel& pix) {
    cout << "R: " << pix.r << ", G: " << pix.g << ", B: " << pix.b;
    return cout;
}

int main(int argc, char* argv[]) {
	
}

