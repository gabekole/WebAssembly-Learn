async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 600;
	const height = 600;

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

	instance.exports.go();

	ctx.putImageData(image, 0, 0);
}


init();
