(async () => {
	const codePromise = fetch('./hello.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);
	
	const buffer = new Uint8Array(instance.exports.memory.buffer);

	const pointer = instance.exports.hello();

	let string = "";
	for(let i = pointer; buffer[i]; i++){
		string += String.fromCharCode(buffer[i])
	}

	console.log(string);
	document.getElementById("container").textContent = string;
})();
