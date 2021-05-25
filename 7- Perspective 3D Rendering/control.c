#include <math.h>
#include <stdlib.h>

#define pointNum 8
#define distance 1.5

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

		while(round(y) != y2){
			addPixel(round(x), round(y), color);
			
			x += dx;
			y += dy;
		}
		addPixel(round(x), round(y), color);
	}
	else{
		double m = (y2-y1)/((double)(x2-x1));
		if(x1 > x2)
			dx = -1;
		else
			dx = 1;

		dy = dx*m;

		while(round(x) != x2){
			addPixel(round(x), round(y), color);
			x += dx;
			y += dy;	
		}
		addPixel(round(x), round(y), color);
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
