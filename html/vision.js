const body = document.getElementById("body")

results.forEach((result, index)=>{

	const h2 = document.createElement("h2");
	h2.innerHTML = "過程：" + (index + 1);
	body.appendChild(h2);

	const table = document.createElement("table");

	result.forEach((array1, index) => {
		let tr = document.createElement("tr");
		array1.forEach((val) => {
			let td = document.createElement("td");
			td.textContent = val;
			td.className = "class"+val;
			tr.appendChild(td);
		})

		table.appendChild(tr)
	});

	body.appendChild(table);
})
