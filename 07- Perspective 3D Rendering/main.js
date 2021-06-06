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

	let update = () => {
		ctx.putImageData(image, 0, 0);
	}
	
	console.log(instance.exports);
	
	while(true){
		await sleep(10);
		update();
		instance.exports.rotateY(.01);
		instance.exports.rotateZ(.01);
		instance.exports.rotateX(.01);
		instance.exports.draw();
	}
}


init();





function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
