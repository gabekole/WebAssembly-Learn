async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 600;
	const height = 600;

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

	let set = color => instance.exports.set(color);
	let update = () => {
		ctx.putImageData(image, 0, 0);
	}
	let drawPixel = (x, y, color) => instance.exports.addPixel(x, y, color);
	let drawLine = (x1, y1, x2, y2, color) => instance.exports.drawLine(x1, y1, x2, y2, color);
	
	console.log(instance.exports);
	
	while(true){
		await sleep(30);
		update();
		instance.exports.rotateY(.05);
		instance.exports.rotateX(.05);
		instance.exports.rotateZ(.05);
		instance.exports.draw();
	}
}


init();





function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
