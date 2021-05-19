async function jsSpeed(){
	let start = new Date().getTime();

	let result = 0;	
	for(let i = 0; i < 42_000_000; i++){
		result += i ** i ** i;
	}

	let end = new Date().getTime();
	let time = (end-start);
	console.log(result);
	
	document.getElementById("result2").textContent = time + " milliseconds";
}