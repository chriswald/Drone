class ControllerStreamer {

	public OnWebSocketMessageReceived: (Uint8Array) => Promise<void>;

	public async StreamControllerData(): Promise<void> {
		let streamer: ControllerStreamer = this;
		let appInsights: AppInsights = new AppInsights();

		let webSocketProtocol: string = (window.location.protocol == "http:" ? "ws://" : "wss://");
		let port: string = (window.location.port.length > 0 ? ":" + window.location.port : "");
		let appName: string = await appInsights.AppName();

		let webSocketUrl: string =
			webSocketProtocol +
			window.location.hostname +
			port +
			"/" + appName +
			"/api/Controller";

		let socket: WebSocket = new WebSocket(webSocketUrl);
		socket.onopen = function (): void { console.log("WebSocket opened"); };
		socket.onmessage = function (event: MessageEvent): void { streamer.ProcessSocketMessage(event.data); };
		socket.onerror = function (event: Event): void { console.error("WebSocket error: " + event); };
		socket.onclose = function (): void { console.log("WebSocket closed"); }
	}

	private async ProcessSocketMessage(data: Blob): Promise<void> {
		let array: Uint8Array = await this.BlobToArray(data);

		if (this.OnWebSocketMessageReceived) {
			this.OnWebSocketMessageReceived(array);
		}
	}

	private BlobToArray(blob: Blob): Promise<Uint8Array> {
		return new Promise<Uint8Array>(resolve => {
			let fileReader: FileReader = new FileReader();
			
			fileReader.onload = function (event: ProgressEvent) {
				let fr: FileReader = event.target as FileReader;
				let result: string | ArrayBuffer = fr.result;

				if (typeof result === "string") {
					resolve(new Uint8Array(0));
				}
				else {
					let array: Uint8Array = new Uint8Array(result);
					resolve(array);
				}
			};

			fileReader.onerror = function (event: ProgressEvent) {
				console.error("Blob read error:");
				console.error(event);
			}

			fileReader.readAsArrayBuffer(blob);
		});
	}
}