async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = instance.exports.getWidth();
	const height = instance.exports.getHeight();

	const canvas = document.getElementById("demo-canvas");
	canvas.width = width;
	canvas.height = height;
	
	console.log(instance.exports);

	const buffer_address = instance.exports.getBuffer();
	const image = new ImageData(
		new Uint8ClampedArray(
			instance.exports.memory.buffer,
			buffer_address,
			4*width*height,
		),
		width,
	);

	const ctx = canvas.getContext("2d");

	console.log(instance);

	let set = color => instance.exports.set(color);
	let update = () => ctx.putImageData(image, 0, 0);
	let drawPixel = (x, y, color) => instance.exports.addPixel(x, y, color);
	let drawLine = (x1, y1, x2, y2, color) => instance.exports.drawLine(x1, y1, x2, y2, color);
	
	set(0xffff0000);
	drawPixel(200, 200, 0xffffffff);
	drawLine(100, 100, 200, 500, 0xff000000);
	update();
}


init();
