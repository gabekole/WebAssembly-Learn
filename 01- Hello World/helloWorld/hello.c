#define WASM_EXPORT __attribute__((visibility("default")))

WASM_EXPORT
int main() {
	return 0;
}


WASM_EXPORT
char* hello() {
	return "Hello World";
}
