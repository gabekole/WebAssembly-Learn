async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 800;
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

	let update = () => ctx.putImageData(image, 0, 0);
	let draw = () => instance.exports.draw();
	
	async function loop() {
		while(true){
			await sleep(10);
			update();
			draw();
		}
	}
	loop();
	
	const a1 = document.getElementById("angle1");
	const a2 = document.getElementById("angle2");
	const setter = document.getElementById("setter");
	
	setter.onclick = () => {
		instance.exports.setAngle(a1.value/100, a2.value/100);
	}
}


init();





function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
