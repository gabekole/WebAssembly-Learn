#include <math.h>
#include <stdlib.h>

#define pointNum 8
#define distance 3

unsigned int WIDTH = 600;
unsigned int HEIGHT = 600;
unsigned int *BUFFER;


int dimensions = 3;
double points[pointNum][3][1] = 
{
	{{.5},{.5},{.5},},
	{{-.5},{.5},{.5},},
	{{-.5},{-.5},{.5},},
	{{.5},{-.5},{.5},},
	
	{{.5},{.5},{-.5},},
	{{-.5},{.5},{-.5},},
	{{-.5},{-.5},{-.5},},
	{{.5},{-.5},{-.5},},
};

double projection[pointNum][3][1];

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


void matrixMultiply(int r1, int c1, double array1[][c1], int r2, int c2, double array2[][c2], double res[r1][c2]){
	if(c1 != r2){
		return;
	}
	
	for (int i = 0; i < r1; i++){
		for(int j = 0; j < c2; j++){
			double sum = 0;
			for(int k = 0; k < c1; k++){
				sum += array1[i][k] * array2[k][j];
			}
			res[i][j] = sum;
		}
	}
}





void apply(int r1, int c1, double array[r1][c1]){
	
	for(int i = 0; i < pointNum; i++){
		double result[r1][1];
		matrixMultiply(r1, c1, array, dimensions, 1, points[i], result);
		for(int j = 0; j < dimensions; j++){
			points[i][j][0] = result[j][0];
		}
	}
}



void rotateZ(double theta){
	double matrix[3][3] = 
	{
		{cos(theta), -sin(theta), 0},
		{sin(theta), cos(theta), 0},
		{0, 0, 1},
	};
	apply(3, 3, matrix);
}

void rotateX(double theta){
	double matrix[3][3] = 
	{
		{1, 0, 0},
		{0, cos(theta), -sin(theta)},
		{0, sin(theta), cos(theta)},
	};
	apply(3, 3, matrix);
}

void rotateY(double theta){
	double matrix[3][3] = 
	{
		{cos(theta), 0, sin(theta)},
		{0, 1, 0},
		{-sin(theta), 0, cos(theta)},
	};
	apply(3, 3, matrix);
}


void project(){

	for(int i = 0; i < pointNum; i++){
		double z = points[i][2][0];
		double a = 1 / ( distance - z );
		
		double oblique[3][3] = 
		{
			{a, 0, 0},
			{0, a, 0},
			{0, 0, 0},
		};	
		matrixMultiply(3, 3, oblique, 3, 1, points[i], projection[i]);
	}
}

void draw(){
	set(0xffffffff);
	
	project();
	
	int factor = 300;
	for(int i = 0; i < 3; i++){
		drawLine((int)(factor*projection[i][0][0])+(WIDTH/2), (int)(factor*projection[i][1][0])+(HEIGHT/2), (int)(factor*projection[i+1][0][0])+(WIDTH/2), (int)(factor*projection[i+1][1][0]+(HEIGHT/2)), 0xffff0000);
	}
	drawLine((int)(factor*projection[3][0][0])+(WIDTH/2), (int)(factor*projection[3][1][0])+(HEIGHT/2), (int)(factor*projection[0][0][0])+(WIDTH/2), (int)(factor*projection[0][1][0]+(HEIGHT/2)), 0xffff0000);
	
	for(int i = 4; i < 7; i++){
		drawLine((int)(factor*projection[i][0][0])+(WIDTH/2), (int)(factor*projection[i][1][0])+(HEIGHT/2), (int)(factor*projection[i+1][0][0])+(WIDTH/2), (int)(factor*projection[i+1][1][0]+(HEIGHT/2)), 0xffff0000);
	}
	drawLine((int)(factor*projection[7][0][0])+(WIDTH/2), (int)(factor*projection[7][1][0])+(HEIGHT/2), (int)(factor*projection[4][0][0])+(WIDTH/2), (int)(factor*projection[4][1][0]+(HEIGHT/2)), 0xffff0000);
	
	
	for(int i = 0; i < 4; i++){
		drawLine((int)(factor*projection[i][0][0])+(WIDTH/2), (int)(factor*projection[i][1][0])+(HEIGHT/2), (int)(factor*projection[i+4][0][0])+(WIDTH/2), (int)(factor*projection[i+4][1][0]+(HEIGHT/2)), 0xffff0000);
	}
}
