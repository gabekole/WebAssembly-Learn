let inst;

async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);
	inst = instance;

	const width = 800;
	const height = 700;

	const canvas = document.getElementById("demo-canvas");
	canvas.width = width;
	canvas.height = height;
	
	const buffer_address = instance.exports.initBuffer(width, height);
	const image = new ImageData(
		new Uint8ClampedArray(
			instance.exports.memory.buffer,
			buffer_address,
			4*width*height,
		),
		width,
	);
	const ctx = canvas.getContext("2d");

	let update = () => ctx.putImageData(image, 0, 0);
	let draw = () => instance.exports.draw();
	
	
	draw();
	update();
}

init();



function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
