function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 64;
	const height = 64;

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
	let addDensity = (x, y, amount) => instance.exports.addDensity(x, y, amount);
	let addVelocity = (x, y, amtX, amtY) => instance.exports.addVelocity(x, y, amtX, amtY);
	
	instance.exports.create(width, 10, 1, .000005);
	
	
	let loop = async () => {
		let angle = 0;
		
		while(true){
			let amp = 50000;
			
			addDensity(32, 32, 1);
			addDensity(31, 32, 1);
			addDensity(33, 32, 1);
			addDensity(32, 31, 1);
			addDensity(32, 33, 1);
			
			addVelocity(32, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(31, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(33, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(32, 31, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(32, 33, Math.cos(angle)*amp, Math.sin(angle)*amp);
			
			instance.exports.step();
			instance.exports.renderD();
			update();
			angle += .1;
			await sleep(100);
		}
	}
	loop();
	
	
}

init();