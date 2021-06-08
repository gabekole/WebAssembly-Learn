function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}

async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 512;
	const height = 512;

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
	
	instance.exports.create(64, 2, 2, .000001);
	
	let angle = 0;
	let loop = async () => {
		let inc = .05;
		
		while(true){
			let amp = 50000;
			
			addDensity(32, 32, .2);
			addDensity(33, 32, .2);
			addDensity(31, 32, .2);
			addDensity(32, 33, .2);
			addDensity(33, 33, .2);
			addDensity(31, 33, .2);
			addDensity(32, 31, .2);
			addDensity(33, 31, .2);
			addDensity(31, 31, .2);
			
			addVelocity(32, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(33, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(31, 32, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(33, 33, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(32, 33, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(31, 33, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(33, 31, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(32, 31, Math.cos(angle)*amp, Math.sin(angle)*amp);
			addVelocity(31, 31, Math.cos(angle)*amp, Math.sin(angle)*amp);
			
			
			
			instance.exports.step();
			instance.exports.renderD();
			update();
			angle += inc;
			
			let rand = Math.random();
			if(rand > .99){
				inc = -inc;
				console.log(inc);
			}
				
			await sleep(20);
		}
	}
	loop();
	
	const range = document.getElementById("angleRange");
	range.oninput = () => {
		angle = range.value / 100.0;
	};
	

}
init();