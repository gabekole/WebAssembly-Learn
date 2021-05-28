let inst;
let drawFractal;
let update;


function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);
	inst = instance;

	const width = 1200;
	const height = 800;

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

	update = () => ctx.putImageData(image, 0, 0);
	drawFractal = (angleIncrement, lengthFactor, color) => instance.exports.drawFractal(angleIncrement, lengthFactor, color);
	
	drawFractal(.9, .6, 0xff000000);
	update();
}

init();