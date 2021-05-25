#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void addPixel(int x, int y){
	printf("(%d, %d)\n", x, y);
}


void drawLine(int x1, int y1, int x2, int y2){
	int rise = y2-y1;
	int run = x2-x1;
	
	if(run == 0){
		if(y2 < y1){
			int temp = y1;
			y1 = y2;
			y2 = temp;
		}
		for (int y = y1; y <= y2; y++){
			addPixel(x1, y);
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
				addPixel(x, y);
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
				addPixel(x, y);
				offset += delta;
				if(offset >= threshold){
					x += adjust;
					threshold += thresholdInc;
				}
			}
		}
	}
}

int main(){
	drawLine(0, 0, 9, -3);
	
	return 0;
}