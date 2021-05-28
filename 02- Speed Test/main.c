#include <math.h>

#define WASM_EXPORT __attribute__((visibility("default")))

int main() {
	return 0;	
}

WASM_EXPORT
int speed() {
  unsigned long long out = 0;
	for(int i = 0; i < 42000000; i++){
    out += pow(i, pow(i, i));	
	}
  return out;
}