async function wasmSpeed(){
	const codePromise = fetch('./main.wasm');
	const { instance } = await WebAssembly.instantiateStreaming(codePromise);
	
	let start = new Date().getTime();

	let result = instance.exports.speed();

	let end = new Date().getTime();
	let time = (end-start);
  console.log(result);

	document.getElementById("result1").textContent = time + " milliseconds";
}