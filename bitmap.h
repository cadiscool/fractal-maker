#include <iostream>
#include <math.h>
#include <cstring>
using namespace std;


class Bitmap{
	public: 
		int width;
		int height;
		int bufferCount; //Bitmaps have a series of buffer bits at the end of each row of pixels
		char* name;
		FILE* f;
		unsigned char* address;
		int fileSize;
		int dataSize;

		Bitmap(int w, int h, char* sname){

			width = w;
			height = h;
			bufferCount = w%4; //We want each row to be a multiple of 4 bits in length, we do so by adding a buffer to the end of each one
			name = sname;

			fileSize = 54 + 3 * (w + (w % 4)) * h; //Defines size of file in bytes, 54 bytes allocated for header data
			dataSize = 3 * (w + (w % 4)) * h; 
			address = (unsigned char*)malloc(3 * (w+ (w % 4)) * h ); //Allocates bytes for the pixel data, typecasts to a pointer to store in address
			memset(address, 0, 3 * (w + (w % 4)) * h );
			f = fopen(sname, "wb");


			//Header templates
			unsigned char bmpfileheader[14] = { 'B','M', 
				0,0,0,0, //These will be set to the size of the file in bytes
				0,0,0,0, //Reserved?
				54,0,0,0 //Start of pixel array
			};
			unsigned char bmpinfoheader[40] = {
				40,0,0,0, //Size of info header
				0,0,0,0, //Image Width
				0,0,0,0, //Image Height
				1,0, //Number of color planes
				24,0 //Bits per pixel, set to 8 bits per pixel (0-256)
			};

			//Adjust headers for filesize, width, and height 
			bmpfileheader[2] = (unsigned char)(fileSize);
			bmpfileheader[3] = (unsigned char)(fileSize >> 8);
			bmpfileheader[4] = (unsigned char)(fileSize >> 16);
			bmpfileheader[5] = (unsigned char)(fileSize >> 24);
			bmpinfoheader[4] = (unsigned char)(w);
			bmpinfoheader[5] = (unsigned char)(w >> 8);
			bmpinfoheader[6] = (unsigned char)(w >> 16);
			bmpinfoheader[7] = (unsigned char)(w >> 24);
			bmpinfoheader[8] = (unsigned char)(h);
			bmpinfoheader[9] = (unsigned char)(h >> 8);
			bmpinfoheader[10] = (unsigned char)(h >> 16);
			bmpinfoheader[11] = (unsigned char)(h >> 24);

			fwrite(bmpfileheader, 1, 14, f);
			fwrite(bmpinfoheader, 1, 40, f);

		}

		void putPixel(int w, int h, int* rgb){
			address[w*3 + ((width*3+bufferCount) * h)] = (unsigned char)rgb[0]; //red
			address[w*3 + ((width*3+bufferCount) * h) + 1] = (unsigned char)rgb[1]; //green
			address[w*3 + ((width*3+bufferCount) * h) + 2] = (unsigned char)rgb[2]; //blue
		}

		void closeBitmap(){
			fwrite(address, 1, dataSize, f);
			free(address);
			fclose(f);
		}
};

void HSVtoRGB(double *hsv, int *rgb) {
	double H = hsv[0];
	double S = hsv[1];
	double V = hsv[2];
	double s = S / 100;
	double v = V / 100;
	double C = s * v;
	double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	double m = v - C;
	double r, g, b;
	if (H >= 0 && H < 60) {
		r = C, g = X, b = 0;
	}
	else if (H >= 60 && H < 120) {
		r = X, g = C, b = 0;
	}
	else if (H >= 120 && H < 180) {
		r = 0, g = C, b = X;
	}
	else if (H >= 180 && H < 240) {
		r = 0, g = X, b = C;
	}
	else if (H >= 240 && H < 300) {
		r = X, g = 0, b = C;
	}
	else {
		r = C, g = 0, b = X;
	}
	int R = (r + m) * 255;
	int G = (g + m) * 255;
	int B = (b + m) * 255;
	rgb[0] = R;
	rgb[1] = G;
	rgb[2] = B;
}