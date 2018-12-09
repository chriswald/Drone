enum ServiceStatus {
	Stopped = 1,
	StartPending = 2,
	StopPending = 3,
	Running = 4,
	ContinuePending = 5,
	PausePending = 6,
	Paused = 7
}

class ServiceMonitor {
	private static GetXmlHttpRequest(action: string): XMLHttpRequest {
		let xhttp: XMLHttpRequest = new XMLHttpRequest();
		xhttp.open("GET", "api/Service/" + action);
		xhttp.setRequestHeader("Accept", "application/json");
		return xhttp;
	}

	public static GetStatus(callback: (string) => void): void {
		let xhttp: XMLHttpRequest = ServiceMonitor.GetXmlHttpRequest("Status");

		xhttp.onreadystatechange = function () {
			if (this.readyState == 4 && this.status == 200) {
				callback(ServiceStatus[this.responseText]);
			}
		}

		xhttp.send();
	}

	public static Start(comId: string): void {
		let xhttp: XMLHttpRequest = ServiceMonitor.GetXmlHttpRequest("Start/" + comId);
		xhttp.send();
	}

	public static Stop(): void {
		let xhttp: XMLHttpRequest = ServiceMonitor.GetXmlHttpRequest("Stop");
		xhttp.send();
	}
}