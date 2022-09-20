#include <iostream>

#include <math.h>


int * HSVtoRGB(float H, float S, float V) {
	float s = S / 100;
	float v = V / 100;
	float C = s * v;
	float X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
	float m = v - C;
	float r, g, b;
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
	int rgb[3] = {R,G,B};
	return rgb;
}


unsigned char clamp(int num) {
	if (num > 255) {
		return 255;
	}
	if (num < 0) {
		return 0;
	}
	return num;
}



int ismandel(double xR, double yI) {

	double zR = 0;
	double zI = 0;
	for (int n = 0; n < 255; n++) {
		if (pow(zR, 2.0) + pow(zI, 2.0) > 4) {
			return n;
		}
		double zRold = zR;
		zR = pow(zR, 2.0) - pow(zI, 2.0)  + xR;
		zI = (2.0 * zRold * zI) + yI;

	}
	return 0;
}

int main() {

	int w = 8000;
	int h = 8000;
	float rW = (float)w;
	float rH = (float)h;

	//Initialize shit
	FILE* f;
	unsigned char* img = NULL; //Creates pointer towards start of image mem address
	int filesize = 54 + 3 * (w + (w % 4)) * h; //Defines size of file in bytes, 54 bytes allocated for header data
	img = (unsigned char*)malloc(3 * (w+ (w % 4)) * h ); //Allocates bytes for the pixel data, typecasts to a pointer to store in img
	memset(img, 0, 3 * (w + (w % 4)) * h );


	//Utility arrays
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
	//Generate the shit
	long int current_count = 0;
	/*

	img[0] = (unsigned char)(255); //B
	img[1] = (unsigned char)(255); //G
	img[2] = (unsigned char)(255); //R
	current_count += 3;
	img[3] = (unsigned char)(255);
	img[4] = (unsigned char)(255);
	img[5] = (unsigned char)(255);
	current_count += 3;
	img[6] = (unsigned char)(00);
	img[7] = (unsigned char)(00);
	//img[8] = (unsigned char)(00);
	current_count += 2;
	img[8] = (unsigned char)(255);
	img[9] = (unsigned char)(255);
	img[10] = (unsigned char)(255);
	current_count += 3;
	img[11] = (unsigned char)(255);
	img[12] = (unsigned char)(255);
	img[13] = (unsigned char)(255);
	current_count += 3;
	img[14] = (unsigned char)(00);
	img[15] = (unsigned char)(00);
	current_count += 2;
	*/
	
	for (int yaxis = 0; yaxis < h; yaxis++) { 
		//img[++current_count] = (unsigned char)(255);
		//img[++current_count] = (unsigned char)(0);
		//img[++current_count] = (unsigned char)(0);
		for (int i = 0; i < w; i++) {
			int x = i;
			int y = yaxis;
			int rX = x - w/2;
			//std::cout << rX << std::endl;
			int rY = y - h/2;
			float mandy = ismandel(0.0004*rX, 0.0004*rY);
			//std::cout << x << std::endl;
			int* rgb = HSVtoRGB(mandy, 75, (mandy*-1 + 100));
			img[current_count++] = clamp(1.2*rgb[2]); //B
			img[current_count++] = clamp(0.2*rgb[1]); //G
			img[current_count++] = clamp(0.4*rgb[0]); //R
			

		}
		if ((w % 4) != 0) {
			for(int e = 0; e<(w%4); e++){ img[current_count++] = (unsigned char)(0); }
		}
	}
	
	//std::cout << "HERE" << std::endl;
	//Fixe the file header
	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(w);
	bmpinfoheader[5] = (unsigned char)(w >> 8);
	bmpinfoheader[6] = (unsigned char)(w >> 16);
	bmpinfoheader[7] = (unsigned char)(w >> 24);
	bmpinfoheader[8] = (unsigned char)(h);
	bmpinfoheader[9] = (unsigned char)(h >> 8);
	bmpinfoheader[10] = (unsigned char)(h >> 16);
	bmpinfoheader[11] = (unsigned char)(h >> 24);

	//write the fucking data
	fopen_s(&f, "red.bmp", "wb");
	fwrite(bmpfileheader, 1, 14, f);
	fwrite(bmpinfoheader, 1, 40, f);
	fwrite(img, 1,current_count, f);
	free(img);
	std::cout << w % 4 << std::endl;
	fclose(f);
	
}