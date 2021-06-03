#include <math.h>
#include <stdlib.h>


unsigned int WIDTH = 800;
unsigned int HEIGHT = 800;
unsigned int *BUFFER;

double deltaAngle = .9;
double lFactor = .67;
double orientation = 0;
int ypos = 500;


const unsigned int* initBuffer(int width, int height){
	WIDTH = width;
	HEIGHT = height;
	BUFFER = malloc(sizeof(unsigned int) * WIDTH * HEIGHT);
	
	return BUFFER;
}

void set(unsigned int color) {
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y<HEIGHT; y++){
			BUFFER[WIDTH*y + x] = color;
		}	
	}	
}

void addPixel(int x, int y, unsigned int color){
	if(x >= WIDTH || y >= HEIGHT || (WIDTH*y + x) >= WIDTH*HEIGHT || (WIDTH*y + x) < 0){
		return;
	}
	BUFFER[WIDTH*y + x] = color;
}

void fillSquare(int x0, int y0, int width, unsigned int color){
	for(int y = y0; y < width+y0; y++){
		for(int x = x0; x < width+x0; x++){
			addPixel(x, y, color);
		}
	}		
}