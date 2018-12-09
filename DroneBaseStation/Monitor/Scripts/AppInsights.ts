class AppInsights {
	public AppName(): Promise<string> {
		return new Promise<string>(resolve => {
			let xhttp: XMLHttpRequest = AppInsights.GetXmlHttpRequest("AppName");

			xhttp.onreadystatechange = function () {
				if (this.readyState == 4 && this.status == 200) {
					resolve(JSON.parse(this.responseText));
				}
			}

			xhttp.send();
		});
	}

	private static GetXmlHttpRequest(action: string): XMLHttpRequest {
		let xhttp: XMLHttpRequest = new XMLHttpRequest();
		xhttp.open("GET", "api/AppInsights/" + action);
		xhttp.setRequestHeader("Accept", "application/json");
		return xhttp;
	}
}