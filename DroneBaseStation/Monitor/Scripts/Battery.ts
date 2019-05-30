enum BatteryLevel {
	Empty,
	Low,
	Medium,
	Full
}

class Battery {
	private _tagId: string;
	private _level: BatteryLevel;
	private _batteryName: string;
	private _percent: number;

	public constructor(batteryName: string, tagId: string, level: BatteryLevel) {
		this._batteryName = batteryName;
		this._tagId = tagId;
		this._level = level;

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

		this.Level = level;
	}

	public get Percent(): number {
		return this._percent;
	}

	public get Level(): BatteryLevel {
		return this._level;
	}

	public set Level(level: BatteryLevel) {
		this._level = level;
		let tag: HTMLElement = document.getElementById(this._tagId);
		let use: SVGUseElement = tag.getElementsByTagName("use")[0];
		use.setAttribute("href", "#" + Battery.BatteryLevelToString(level));
	}

	private static BatteryLevelToString(level: BatteryLevel): string {
		switch (level) {
			case BatteryLevel.Empty: return "batteryempty";
			case BatteryLevel.Low: return "batterylow";
			case BatteryLevel.Medium: return "batterymedium";
			case BatteryLevel.Full: return "batteryfull";
		}
	}

	private static GetXmlHttpRequest(action: string): XMLHttpRequest {
		let xhttp: XMLHttpRequest = new XMLHttpRequest();
		xhttp.open("GET", "api/Power/" + action);
		xhttp.setRequestHeader("Accept", "application/json");
		return xhttp;
	}

	public GetBatteryLevel(): void {
		let xhttp: XMLHttpRequest = Battery.GetXmlHttpRequest(this._batteryName);
		let battery: Battery = this;

		xhttp.onreadystatechange = function () {
			if (this.readyState == 4 && this.status == 200) {
				battery._percent = +this.responseText;
				battery.Level = Battery.BatteryPercentToLevel(battery._percent);
			}
		}

		xhttp.send();
	}

	public static BatteryPercentToLevel(percent: number): BatteryLevel {
		if (percent < 10) {
			return BatteryLevel.Empty;
		}
		else if (percent < 40) {
			return BatteryLevel.Low;
		}
		else if (percent < 85) {
			return BatteryLevel.Medium;
		}
		else {
			return BatteryLevel.Full;
		}
	}
}