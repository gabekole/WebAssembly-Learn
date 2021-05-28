#include <math.h>
#include <stdlib.h>

unsigned int WIDTH = 600;
unsigned int HEIGHT = 600;
unsigned int *BUFFER;


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
	if((WIDTH*y + x) >= WIDTH*HEIGHT || (WIDTH*y + x) < 0)
		return;
	BUFFER[WIDTH*y + x] = color;
}

void drawLine(int x1, int y1, int x2, int y2, unsigned int color){

	double dx, dy;
	double y = y1;
	double x = x1;
	
	if(x2-x1 == 0 || fabs((y2-y1)/((double)(x2-x1))) > 1){
		double m = (x2-x1)/((double)(y2-y1));	
		if(y1 > y2)
			dy = -1;
		else
			dy = 1;

		dx = dy*m;

		do{
			addPixel(round(x), round(y), color);
			
			x += dx;
			y += dy;
		} while((int)y != y2);
		addPixel(round(x), round(y), color);
	}
	else{
		double m = (y2-y1)/((double)(x2-x1));
		if(x1 > x2)
			dx = -1;
		else
			dx = 1;

		dy = dx*m;

		do{
			addPixel(round(x), round(y), color);
			x += dx;
			y += dy;	
		}while((int)x != x2);
		addPixel(round(x), round(y), color);
	}
}
