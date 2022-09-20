#include <iostream>
#include "bitmap.h"
#include <math.h>
#include <chrono>
using namespace std;


unsigned char clamp(int num) {
	return (num > 255)*255 + (num > 255)*-1*num + (num < 0)*-1*num + num;
}

int mandelGen(double xR, double yI) {
	double zR = 0;
	double zI = 0;
	for (int n = 0; n < 100; n++) {
		double powR = zR * zR;
		double powI = zI * zI;
		if (powR+ powI > 4) {
			return n;
		}
		zI = (2.0 * zR * zI) + yI;
		zR = powR - powI  + xR;

	}
	return 255;
}

int cplxCos(double xR, double yI){
	double zR = 0;
	double zI = 0;
	for (int n = 0; n < 100; n++) {
		double powR = zR * zR;
		double powI = zI * zI;
		if (powR+ powI > 1000) {
			return n;
		}
		double ziold = zI;
		zI = cos(zR)*cosh(zI) + yI;
		zR = -1* sin(zR) * sinh(ziold) + xR;

	}

	return 255;
}



int main() {
	auto start = std::chrono::high_resolution_clock::now();
	int width = 10000; //Initialize image dimensions
	int height = 10000;
	double canvas[4] = {0.836,0.837, 0.222, 0.223 };
	//double canvas[4] = {0.45,0.65, 0.84,1.05 }; // x1, x2, y1, y2  Defines a canvas to work on, bottom left corner is x1, y1
	char* name = "cmplxCos2-10000x10000.bmp";
	Bitmap image = Bitmap(width, height, name); //Creates a new bitmap image
	double deltaX = (canvas[1]- canvas[0]) / double(width); //Calculates x and y increment for each pixel 
	double deltaY = (canvas[3]- canvas[2]) / double(height);

	for(int h = 0; h<height; h++){
		double y = h*deltaY + canvas[2];
		for(int w = 0; w<width; w++){
			double x = w*deltaX + canvas[0]; //Calculate x and y coordinates

			double hue = clamp(2.55*cplxCos(x, y)); //Use fractal to generate a hue, note that this is clamped below 255

			double hsv[3] = {hue, 50, 50}; //create an hsv array
			int rgb[3]; //create an rgb array
			HSVtoRGB(hsv, rgb); //converts contents of hsv array, stores them in pointer from rgb array 
			image.putPixel(w,h,rgb); //write pixel to memory 

		
		}

	}

	image.closeBitmap(); //writes data to memory

	auto finish = std::chrono::high_resolution_clock::now();
    auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start);
    std::cout << milliseconds.count() << " milliseconds\n";
	return 0;
	
}