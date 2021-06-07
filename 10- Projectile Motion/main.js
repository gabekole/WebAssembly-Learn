let angleRange = document.getElementById("angleRange");
let velocityRange = document.getElementById("velocityRange");
let gravityRange = document.getElementById("gravityRange");
let launcher = document.getElementById("launchButton");

angleRange.oninput = () => {
	document.getElementById("cangle").textContent = "Angle: " + angleRange.value/100.0;
}

velocityRange.oninput = () => {
	document.getElementById("cvelocity").textContent = "Velocity: " + velocityRange.value;
}

gravityRange.oninput = () => {
	document.getElementById("cgravity").textContent = "Gravity: " + gravityRange.value;
}

async function init() {
	const codePromise = fetch('./control.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);

	const width = 1500;
	const height = 1000;

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
	let drawFloor = () => instance.exports.drawLine(0, height/2, width, height/2, 0xffff0000);
	
	let launchProjectile = async (angle, velocity, gravity) => {
		const begin = performance.now();
		let current = performance.now();
		
		const timeToComplete = (2*velocity*Math.sin(angle))/gravity;
		
		while(current - begin <= timeToComplete*1000){
			await sleep(20);
			instance.exports.set(0xffffffff);
			drawFloor();
			instance.exports.drawProjectile(angle, velocity, gravity, (current - begin)/1000.0);
			update();
			current = performance.now();
		}
	}
	
	launcher.onclick = () => {
		instance.exports.set(0xffffffff);
		drawFloor();
		
		let angle = angleRange.value/100.0;
		let velocity = velocityRange.value;
		let gravity = gravityRange.value;
	
		launchProjectile(angle, velocity, gravity);
	}
	
	drawFloor();
}


init();





function sleep(ms) {
  return new Promise(resolve => setTimeout(resolve, ms));
}
