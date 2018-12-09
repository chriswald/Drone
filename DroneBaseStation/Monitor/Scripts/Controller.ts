enum GamepadButtonFlags {
	None = 0,
	DPadUp = 1,
	DPadDown = 2,
	DPadLeft = 4,
	DPadRight = 8,
	Start = 16,
	Back = 32,
	LeftThumb = 64,
	RightThumb = 128,
	LeftShoulder = 256,
	RightShoulder = 512,
	A = 4096,
	B = 8192,
	X = 16384,
	Y = 32768
}

class Controller {

	public readonly FloatSize: number = 4;
	public readonly IntSize: number = 4;
	public readonly ByteSize: number = 1;

	public readonly TriggerMin: number = 0;
	public readonly TriggerMax: number = 255;
	public readonly StickMin: number = 0;
	public readonly StickMax: number = 100;

	private _leftStickX: number;
	private _leftStickY: number;
	private _rightStickX: number;
	private _rightStickY: number;
	private _leftTrigger: number;
	private _rightTrigger: number;
	private _buttonFlags: GamepadButtonFlags;

	private _streamer: ControllerStreamer;
	private _visualizer: ControllerVisualization;

	public get LeftStickX(): number { return this._leftStickX; }
	public get LeftStickY(): number { return this._leftStickY; }
	public get RightStickX(): number { return this._rightStickX; }
	public get RightStickY(): number { return this._rightStickY; }
	public get LeftTrigger(): number { return this._leftTrigger; }
	public get RightTrigger(): number { return this._rightTrigger; }
	public get ButtonFlags(): GamepadButtonFlags { return this._buttonFlags; }
	
	public constructor(tagId?: string) {
		let controller: Controller = this;
		this._streamer = new ControllerStreamer();
		this._streamer.OnWebSocketMessageReceived = async (bytes: Uint8Array): Promise<void> => {
			controller.OnWebSocketMessageReceived(bytes);
		};

		if (tagId) {
			this._visualizer = new ControllerVisualization(tagId);
			this._visualizer.UpdateVisualization(this);
		}
	}

	public BeginStreaming() {
		this._streamer.StreamControllerData();
	}

	public ParseByteArray(bytes: Uint8Array): void {
		let byteIndex: number = 0;

		byteIndex += 2; // Skip the size byte and separator

		this._leftStickX = this.GetFloat(bytes, byteIndex);
		byteIndex += this.FloatSize + 1; // Move past this float

		this._leftStickY = this.GetFloat(bytes, byteIndex);
		byteIndex += this.FloatSize + 1; // Move past this float

		this._rightStickX = this.GetFloat(bytes, byteIndex);
		byteIndex += this.FloatSize + 1; // Move past this float

		this._rightStickY = this.GetFloat(bytes, byteIndex);
		byteIndex += this.FloatSize + 1; // Move past this float

		this._leftTrigger = this.GetByte(bytes, byteIndex);
		byteIndex += this.ByteSize + 1; // Move past this byte

		this._rightTrigger = this.GetByte(bytes, byteIndex);
		byteIndex += this.ByteSize + 1; // Move past this byte

		this._buttonFlags = this.GetInt(bytes, byteIndex);
		byteIndex += this.IntSize + 1; // Move past this int

		// Battery level exists here but that's retrieved a different way
	}

	public ButtonIsPressed(flagsToTest: GamepadButtonFlags): boolean {
		return !!(this._buttonFlags & flagsToTest);
	}

	private async OnWebSocketMessageReceived(bytes: Uint8Array): Promise<void> {
		this.ParseByteArray(bytes);

		if (this._visualizer) {
			this._visualizer.UpdateVisualization(this);
		}

		if (this.ButtonIsPressed(GamepadButtonFlags.LeftShoulder)) {
			console.log("Left Shoulder");
		}
		else if (this.ButtonIsPressed(GamepadButtonFlags.DPadDown)) {
			console.log("D Pad Down");
		}
	}

	private GetFloat(bytes: Uint8Array, startByte: number): number {
		let view: DataView = this.GetSliceView(bytes, startByte, this.FloatSize);
		return view.getFloat32(0);
	}

	private GetInt(bytes: Uint8Array, startByte: number): number {
		let view: DataView = this.GetSliceView(bytes, startByte, this.IntSize);
		return view.getInt32(0);
	}

	private GetByte(bytes: Uint8Array, startByte: number): number {
		let view: DataView = this.GetSliceView(bytes, startByte, this.ByteSize);
		return view.getUint8(0);
	}

	private GetSliceView(bytes: Uint8Array, startByte: number, sliceSize: number): DataView {
		let slice: Uint8Array;
		let buffer: ArrayBuffer = new ArrayBuffer(sliceSize);
		let view: DataView = new DataView(buffer);

		slice = bytes.slice(startByte, startByte + sliceSize);
		slice.forEach(function (value, index) {
			// Use sliceSize - 1 - index to swap byte order
			view.setUint8(sliceSize - 1 - index, value);
		});

		return view;
	}
}