#include <string>


class Bitmap{
	public: 
		int width;
		int height;
		string name;
		FILE* f;
		unsigned char* address;
		int filesize;

		void Bitmap(int w, int h, string sname){

			width = w;
			height = h;
			name = sname + ".bmp";

			filesize = 54 + 3 * (w + (w % 4)) * h; //Defines size of file in bytes, 54 bytes allocated for header data
			address = (unsigned char*)malloc(3 * (w+ (w % 4)) * h ); //Allocates bytes for the pixel data, typecasts to a pointer to store in img
			memset(img, 0, 3 * (w + (w % 4)) * h );
			fopen_s(&f, sname, "wb");


			//Write the headers 
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

			fwrite(bmpfileheader, 1, 14, f);
			fwrite(bmpinfoheader, 1, 40, f);

		}

		~Bitmap(){

		}

		int putPixel(int w, int h, int[3] rgb){
			img[]
		}
}