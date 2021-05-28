#include <math.h>
#include <stdlib.h>


unsigned int WIDTH = 800;
unsigned int HEIGHT = 800;
unsigned int *BUFFER;


//center
int cx = 400;
int cy = 350;

//mass and length of rod and bob
double r1 = 200;
double r2 = 200;
double m1 = 20;
double m2 = 20;

//initial angle
double a1 = -.7;
double a2 = 1.4;

//initial derivatives
double av1 = 0;
double av2 = 0;
double aa1 = 0;
double aa2 = 0;

//gravity
double g = 1;

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

void drawPendulum(){
	double x1 = r1 * sin(a1);
	double y1 = r1 * cos(a1);
	drawLine(cx, cy, round(cx+x1), round(cy+y1), 0xffff0000);
	drawCircle(round(cx+x1), round(cy+y1), m1, 0xff000000);
	
	double x2 = r2 * sin(a2);
	double y2 = r2 * cos(a2);
	drawLine(round(cx+x1), round(cy+y1), round(cx+x1+x2), round(cy+y1+y2), 0xff00ff55); 
	drawCircle(round(cx+x1+x2), round(cy+y1+y2), m2, 0xff0f0f0f);
}

void calculateAcceleration(){
	double numerator1 = ((-g)*(2*m1 + m2)*sin(a1)) - (m2*g*sin(a1-(2*a2))) - (2*sin(a1-a2)*m2)*(av2*av2*r2 + av1*av1*r1*cos(a1-a2));
	double denominator1 = r1*(2*m1 + m2 -(m2*cos(2*a1-2*a2)));
	
	aa1 = numerator1/denominator1;
	
	double numerator2 = (2*sin(a1-a2)) * ((av1*av1*r1*(m1+m2)) + (g*(m1+m2)*cos(a1)) + (av2*av2*r2*m2*cos(a1-a2)));
	double denominator2 = r2*(2*m1 + m2 - m2*cos(2*a1 - 2*a2));
	
	aa2 = numerator2/denominator2;
}

void draw(){
	calculateAcceleration();
	av1 += aa1;
	av2 += aa2;
	
	a1 += av1;
	a2 += av2;
	
	
	set(0xffffffff);
	drawPendulum();
}