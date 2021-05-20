const unsigned int WIDTH = 600;
const unsigned int HEIGHT = 600;
unsigned int BUFFER[WIDTH * HEIGHT];

const unsigned int* getBuffer(){
	return BUFFER;
}

void draw() {
	unsigned int screen[HEIGHT][WIDTH];

	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y<HEIGHT; y++){
			unsigned int color = 0xffff0000;
			screen[x][y] = color;
			BUFFER[HEIGHT * x + y] = screen[x][y];
		}	
	}	
}
