class ConnectedDevice {
	private _tagId: string;
	private _deviceType: string;
	private _isConnected: boolean;

	public get IsConnected(): boolean { return this._isConnected; }

	public set IsConnected(isConn: boolean) {
		this._isConnected = isConn;
		let tag: HTMLElement = document.getElementById(this._tagId);
		let use: SVGUseElement = tag.getElementsByTagName("use")[0];
		let isConnString: string = (this._isConnected ? "connected" : "");
		use.setAttribute("href", "#" + isConnString);
	}

	public constructor(deviceType: string, tagId: string) {
		this._deviceType = deviceType;
		this._tagId = tagId;

		let tag: HTMLElement = document.getElementById(this._tagId);
		let tagClass: string = tag.className;

		let svg: SVGSVGElement = document.createElementNS("http://www.w3.org/2000/svg", "svg");
		svg.classList.add("svg");

		if (this._tagId !== "") {
			svg.id = this._tagId;
		}

		if (tagClass !== "") {
			svg.classList.add(tagClass);
		}

		let use: SVGUseElement = document.createElementNS("http://www.w3.org/2000/svg", "use");
		use.setAttribute("href", "#");

		svg.appendChild(use);

		tag.parentNode.replaceChild(svg, tag);
	}

	public GetConnectedStatus(): void {
		let xhttp: XMLHttpRequest = ConnectedDevice.GetXmlHttpRequest(this._deviceType);
		let device: ConnectedDevice = this;

		xhttp.onreadystatechange = function () {
			if (this.readyState == 4 && this.status == 200) {
				device.IsConnected = device.BoolTryParse(this.responseText);
			}
		}

		xhttp.send();
	}

	private static GetXmlHttpRequest(action: string): XMLHttpRequest {
		let xhttp: XMLHttpRequest = new XMLHttpRequest();
		xhttp.open("GET", "api/Connection/" + action);
		xhttp.setRequestHeader("Accept", "application/json");
		return xhttp;
	}

	private BoolTryParse(input: string): boolean {
		try {
			return JSON.parse(input);
		}
		catch (e) {
			return false;
		}
	}
}