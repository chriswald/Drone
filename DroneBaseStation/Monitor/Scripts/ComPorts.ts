class ComPorts {
	private static GetXmlHttpRequest(): XMLHttpRequest {
		let xhttp: XMLHttpRequest = new XMLHttpRequest();
		xhttp.open("GET", "api/ComPort");
		xhttp.setRequestHeader("Accept", "application/json");
		return xhttp;
	}

	public static List(callback: (string) => void): void {
		let xhttp: XMLHttpRequest = ComPorts.GetXmlHttpRequest();

		xhttp.onreadystatechange = function () {
			if (this.readyState == 4 && this.status == 200) {
				callback(this.responseText);
			}
		}

		xhttp.send();
	}

	public static GetSelectedPortId(): string {
		let comList: HTMLSelectElement = document.getElementById("comList") as HTMLSelectElement;
		return comList.options[comList.selectedIndex].text;
	}
}