#include <math.h>
#include <stdlib.h>


unsigned int WIDTH = 800;
unsigned int HEIGHT = 800;
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
	if(x >= WIDTH || y >= HEIGHT || (WIDTH*y + x) >= WIDTH*HEIGHT || (WIDTH*y + x) < 0){
		return;
	}
	BUFFER[WIDTH*y + x] = color;
}

void drawLine(int x1, int y1, int x2, int y2, unsigned int color){
	int rise = y2-y1;
	int run = x2-x1;
	
	if(run == 0){
		if(y2 < y1){
			int temp = y1;
			y1 = y2;
			y2 = temp;
		}
		for (int y = y1; y <= y2; y++){
			addPixel(x1, y, color);
		}
	}
	else{
		double m = ((double)rise)/run;
		int adjust = 1;
		if ( m < 0 )
			adjust = -1;
			
		int offset = 0;
		
		if ( m <=1 && m >= -1) {
			
			int delta = abs(rise)*2;
			int threshold = abs(run);
			int thresholdInc = abs(run)*2;
			
			int y = y1;
			if ( x2 < x1 ){
				int temp = x1;
				x1 = x2;
				x2 = temp;
				y = y2;
			}
			for(int x = x1; x <= x2; x++){
				addPixel(x, y, color);
				offset += delta;
				if( offset >= threshold ) {
						y += adjust;
						threshold += thresholdInc;
				}
			}
		}
		else {
			int delta = abs(run)*2;
			int threshold = abs(rise);
			int thresholdInc = abs(rise)*2;
			
			int x = x1;
			if(y2 < y1){
				int temp = y1;
				y1 = y2;
				y2 = temp;
				x = x2;
			}
			for(int y = y1; y <= y2; y++){
				addPixel(x, y, color);
				offset += delta;
				if(offset >= threshold){
					x += adjust;
					threshold += thresholdInc;
				}
			}
		}
	}
}

void drawCircle(int mx, int my, int radius, unsigned int color){
	int x = 0;
	int y = radius;
	double d = 1.25 - radius;
	while(x < y)
	{
		if(d < 0)
		{
			d = d + 2 * x + 3;
			x += 1;
		}
		else
		{
			d = d + 2 * (x-y) + 5;
			x += 1;
			y -= 1;
		}
		addPixel(mx + x, my + y, color);
		addPixel(mx + x, my - y, color);
		addPixel(mx - x, my + y, color);
		addPixel(mx - x, my - y, color);
		
		addPixel(mx + y, my + x, color);
		addPixel(mx + y, my - x, color);
		addPixel(mx - y, my + x, color);
		addPixel(mx - y, my - x, color);
	}
}

void fillCircle(int mx, int my, int radius, unsigned int color){
	int x = 0;
	int y = radius;
	double d = 1.25 - radius;
	
	drawLine(mx - radius, my, mx + radius, my, color);
	while(x <= y)
	{
		if(d < 0)
		{
			d = d + 2 * x + 3;
			x += 1;
		}
		else
		{
			d = d + 2 * (x-y) + 5;
			x += 1;
			y -= 1;
		}
		drawLine(mx - x, my + y, mx + x, my + y, color);
		drawLine(mx - x, my - y, mx + x, my - y, color);
		
		drawLine(mx - y, my - x, mx + y, my - x, color);
		drawLine(mx - y, my + x, mx + y, my + x, color);
	}
}


void drawProjectile(double angle, double velocity, double gravity, double time){
	double x = cos(angle) * velocity * time; 	
	double vy = sin(angle) * velocity;
	double y = vy * time - (gravity*time*time / 2);
	int meter = 50;

	fillCircle(WIDTH/12 + round(x*meter), HEIGHT/2-(round(y*meter)) - 9, 20, 0xff000000);
}
