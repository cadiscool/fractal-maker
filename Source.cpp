#include <iostream>
#include "bitmap.h"
#include <math.h>
using namespace std;


unsigned char clamp(int num) {
	return (num > 255)*255 + (num > 255)*-1*num + (num < 0)*-1*num + num;
}

int mandelGen(double xR, double yI) {

	double zR = 0;
	double zI = 0;
	for (int n = 0; n < 255; n++) {
		if (pow(zR, 2.0) + pow(zI, 2.0) > 4) {
			return n;
		}
		double zRold = zR;
		zR = pow(zR, 2) - pow(zI, 2.0)  + xR;
		zI = (2.0 * zRold * zI) + yI;

	}
	return 255;
}

int main() {
	int width = 10000; //Initialize image dimensions
	int height = 10000;
	double canvas[4] = {0,1, 0,1 }; // x1, x2, y1, y2  Defines a canvas to work on, bottom left corner is x1, y1
	char* name = "pleaseWork.bmp";
	Bitmap image = Bitmap(width, height, name); //Creates a new bitmap image
	double deltaX = (canvas[1]- canvas[0]) / double(width); //Calculates x and y increment for each pixel 
	double deltaY = (canvas[3]- canvas[2]) / double(height);

	for(int h = 0; h<height; h++){
		cout << h <<endl;
		for(int w = 0; w<width; w++){
			double x = w*deltaX + canvas[0]; //Calculate x and y coordinates
			double y = h*deltaY + canvas[2];
			double hue = clamp(18*mandelGen(x, y)); //Use fractal to generate a hue, note that this is clamped below 255
			double hsv[3] = {hue, 50, 50}; //create an hsv array
			int rgb[3]; //create an rgb array
			HSVtoRGB(hsv, rgb); //converts contents of hsv array, stores them in pointer from rgb array 
			image.putPixel(w,h,rgb); //write pixel to memory 
		}

	}

	image.closeBitmap(); //writes data to memory

	return 0;
	
}