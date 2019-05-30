/// <reference path="./Battery.ts" />
/// <reference path="./ConnectedDevice.ts" />
/// <reference path="./Service.ts" />

document.addEventListener("DOMContentLoaded", Main);

function Main(): void {
	// Configure the devices with batteries to monitor
	let controllerBattery: Battery = new Battery("Controller", "controllerBattery", BatteryLevel.Empty);
	let laptopBattery: Battery = new Battery("Laptop", "laptopBattery", BatteryLevel.Empty);
	let radioBattery: Battery = new Battery("Radio", "radioBattery", BatteryLevel.Empty);
	let droneSystemBattery: Battery = new Battery("DroneSystem", "droneSystemBattery", BatteryLevel.Empty);
	let droneMotorBattery: Battery = new Battery("DroneMotor", "droneMotorBattery", BatteryLevel.Empty);

	// Configure the devices that need to be communicated with
	let controllerDevice: ConnectedDevice = new ConnectedDevice("Controller", "controllerStatus");
	let laptopDevice: ConnectedDevice = new ConnectedDevice("Laptop", "laptopStatus");
	let radioDevice: ConnectedDevice = new ConnectedDevice("Radio", "radioStatus");
	let droneSystemDevice: ConnectedDevice = new ConnectedDevice("DroneSystem", "droneSystemStatus");
	let droneMotorDevice: ConnectedDevice = new ConnectedDevice("DroneMotor", "droneMotorStatus");

	ListComPorts();
	CheckServiceStatus();

	UpdateBatteryLevels([
		controllerBattery,
		laptopBattery,
		radioBattery,
		droneSystemBattery,
		droneMotorBattery
	]);

	UpdateConnectedDevices([
		controllerDevice,
		laptopDevice,
		radioDevice,
		droneSystemDevice,
		droneMotorDevice
	]);

	// events
	document.getElementById("refreshPorts").addEventListener("click", ListComPorts);
	document.getElementById("startService").addEventListener("click", StartService);
	document.getElementById("stopService").addEventListener("click", StopService);

	// Begin streaming controller data in
	let controller: Controller = new Controller("controllerVisualization");
	controller.BeginStreaming();
}

function ListComPorts(): void {
	ComPorts.List((response: string): void => {
		let list: Array<string> = JSON.parse(response) as Array<string>;
		let comSelect: HTMLSelectElement = document.getElementById("comList") as HTMLSelectElement;
		ClearOptions(comSelect);

		for (let comId of list)
		{
			let option: HTMLOptionElement = document.createElement("option");
			option.text = comId;
			comSelect.add(option);
		}
	});
}

function ClearOptions(selectList: HTMLSelectElement): void {
	for (let i = selectList.options.length - 1; i >= 0; i--) {
		selectList.remove(i);
	}
}

function CheckServiceStatus(): void {
	ServiceMonitor.GetStatus((response: string): void => {
		document.getElementById("serviceStatus").innerText = response;
	});

	setTimeout(CheckServiceStatus, 1000);
}

function StartService(): void {
	let comId: string = ComPorts.GetSelectedPortId();
	if (comId) {
		ServiceMonitor.Start(comId);
	}
}

function StopService(): void {
	ServiceMonitor.Stop();
}

function UpdateBatteryLevels(batteries: Array<Battery>): void {
	for (let battery of batteries) {
		battery.GetBatteryLevel();
	}

	setTimeout(UpdateBatteryLevels, 1 * 1000, batteries);
}

function UpdateConnectedDevices(devices: Array<ConnectedDevice>): void {
	for (let device of devices) {
		device.GetConnectedStatus();
	}

	setTimeout(UpdateConnectedDevices, 1 * 1000, devices);
}