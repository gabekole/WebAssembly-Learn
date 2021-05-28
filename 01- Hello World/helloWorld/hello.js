(async () => {
	const codePromise = fetch('./hello.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);
	
	//This is the memory buffer that is written to by the C functiosn
	const buffer = new Uint8Array(instance.exports.memory.buffer);

	//pointer to the start of the string in the buffer after the function is called
	const pointer = instance.exports.hello();

	//We have to iterate through the buffer until we reach the terminating character to access our string
	let string = "";
	for(let i = pointer; buffer[i]; i++){
		string += String.fromCharCode(buffer[i])
	}

	//Adding the text to the page
	document.getElementById("container").textContent = string;
})();
