class ControllerVisualization {
	private _tagId: string;
	private _canvas: HTMLCanvasElement;
	private _ctx: CanvasRenderingContext2D;

	public constructor(tagId: string) {
		this._tagId = tagId;

		let tag: HTMLElement = document.getElementById(this._tagId);

		this._canvas = document.createElement("canvas");
		this._canvas.width = tag.offsetWidth;
		this._canvas.height = tag.offsetHeight;

		tag.appendChild(this._canvas);

		if (this._canvas.getContext) {
			this._ctx = this._canvas.getContext("2d");
		}
		else {
			console.error("Cannot set the canvas context");
		}
	}

	public UpdateVisualization(controller: Controller): void {
		if (!this._ctx) { return; }

		// Clear the canvas;
		this._ctx.clearRect(0, 0, this._canvas.width, this._canvas.height);

		this.DrawLeftTrigger(controller);
		this.DrawLeftStickGraph(controller);
		this.DrawRightStickGraph(controller);
		this.DrawRightTrigger(controller);
	}

	private DrawLeftTrigger(controller: Controller): void {
		let triggerPercent: number = this.GetTriggerPercent(controller.LeftTrigger, controller);
		this.DrawTriggerBar(0, triggerPercent, .75);
	}

	private DrawRightTrigger(controller: Controller): void {
		let triggerPercent: number = this.GetTriggerPercent(controller.RightTrigger, controller);
		this.DrawTriggerBar(this._canvas.width - 20, triggerPercent, .75);
	}

	private DrawTriggerBar(x: number, triggerPercent: number, scaleHeightPercent?: number, barBottomY?: number): void {
		if (!barBottomY) {
			barBottomY = this._canvas.height;
		}

		let scaledHeight: number = barBottomY;
		if (scaleHeightPercent) { scaledHeight = scaledHeight * scaleHeightPercent; }

		let triggerBarLength: number = (scaledHeight) * triggerPercent;
		let barTopY: number = barBottomY - scaledHeight;

		this._ctx.fillStyle = "rgb(225, 225, 225)";
		this._ctx.fillRect(x, barTopY, 20, scaledHeight);
		this._ctx.fillStyle = "rgb(200, 0, 0)";
		this._ctx.fillRect(x, barBottomY - triggerBarLength, 20, triggerBarLength);
	}

	private GetTriggerPercent(trigger: number, controller: Controller): number {
		let triggerPercent: number = trigger / (controller.TriggerMax - controller.TriggerMin);
		return triggerPercent;
	}

	private DrawLeftStickGraph(controller: Controller): void {
		let sx: number = controller.LeftStickX;
		let sy: number = controller.LeftStickY;
		this.DrawStickGraph(150, this._canvas.height / 2, this._canvas.height, sx, sy, controller, .95);
	}

	private DrawRightStickGraph(controller: Controller): void {
		let sx: number = controller.RightStickX;
		let sy: number = controller.RightStickY;
		this.DrawStickGraph(this._canvas.width - 150, this._canvas.height / 2, this._canvas.height, sx, sy, controller, .95);
	}

	private DrawStickGraph(x: number, y: number, width: number, sx: number, sy: number, controller: Controller, scalePercent?:number): void {
		let scaledWidth: number = width;
		if (scalePercent) { scaledWidth = scaledWidth * scalePercent; }

		let maxRadius: number = scaledWidth / 2;

		let spx: number = this.GetStickPercent(sx, controller);
		let spy: number = this.GetStickPercent(sy, controller);

		this._ctx.beginPath();
		this._ctx.strokeStyle = "rgb(200, 200, 200)"
		this.AddCircleToPath(x, y, maxRadius);
		this.AddCircleToPath(x, y, maxRadius * .75);
		this.AddCircleToPath(x, y, maxRadius * .5);
		this.AddCircleToPath(x, y, maxRadius * .25);
		this.AddCrossHairToPath(x, y, scaledWidth);
		this._ctx.stroke();

		this.AddLineToStickPosition(x, y, scaledWidth, spx, spy);
	}

	private AddCircleToPath(x: number, y: number, r: number): void {
		this._ctx.moveTo(x + r, y);
		this._ctx.arc(x, y, r, 0, Math.PI * 2);
	}

	private AddCrossHairToPath(x: number, y: number, width: number): void {
		this._ctx.moveTo(x, y - (width / 2));
		this._ctx.lineTo(x, y + (width / 2));
		this._ctx.moveTo(x - (width / 2), y);
		this._ctx.lineTo(x + (width / 2), y);
	}

	private AddLineToStickPosition(x: number, y: number, width: number, spx: number, spy: number) {
		let maxRadius: number = width / 2;

		let stickFinalX: number = x + (maxRadius * spx);
		let stickFinalY: number = y - (maxRadius * spy); // note y coords are inverted
		
		this._ctx.beginPath();
		this._ctx.strokeStyle = "rgb(0, 0, 0)";
		this._ctx.moveTo(x, y);
		this._ctx.lineTo(stickFinalX, stickFinalY);
		this._ctx.stroke();

		this._ctx.beginPath();
		this._ctx.fillStyle = "rgb(200, 200, 200)";
		this.AddCircleToPath(x, y, 3);
		this._ctx.fill();

		this._ctx.beginPath();
		this._ctx.fillStyle = "rgb(200, 0, 0)";
		this.AddCircleToPath(stickFinalX, stickFinalY, 3);
		this._ctx.fill();
	}

	private GetStickPercent(stick: number, controller: Controller): number {
		let stickPercent: number = stick / (controller.StickMax - controller.StickMin);
		return stickPercent;
	}
}