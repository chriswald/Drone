using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
using DroneControllerHardware;

namespace Monitor.Service
{
	public class DroneControllerService
	{
		private SerialPort _comPort;

		private DroneController _controller;
		private bool _controllerIsConnected;

		private volatile bool _serviceDone;

		private Thread _workerThread;

		public bool IsRunning { get; private set; }

		public DroneControllerService(int comPortId)
		{
			string portName = "COM" + comPortId;

			try
			{
				_comPort = new SerialPort(portName, 9600, Parity.None, 8, StopBits.One);
				_comPort.DtrEnable = true;
				_comPort.DataReceived += this.ComDataReceived;
				_comPort.Open();
			}
			catch (Exception e)
			{
				string message = $"Error connecting to COM port {portName}: {e.Message}";
				EventLog.WriteEntry(this.GetType().Name, message, EventLogEntryType.Error);
				throw new ArgumentException(message, nameof(comPortId));
			}

			_controller = new DroneController();
		}

		public void Start()
		{
			if (_comPort != null)
			{
				IsRunning = true;
				_workerThread = new Thread(SendControllerData);
				_workerThread.Start();
			}
			else
			{
				throw new InvalidOperationException("Cannot restart the service after it has been stopped. Create a new instance and start that instead");
			}
		}

		public void Stop()
		{
			_serviceDone = true;

			if (_workerThread != null)
			{
				_workerThread.Join();
			}

			_comPort.Close();
			_comPort = null;
			IsRunning = false;
		}

		private void SendControllerData()
		{
			while (!_serviceDone)
			{
				if (_controller.IsConnected)
				{
					if (!_controllerIsConnected)
					{
						_controllerIsConnected = true;
						EventLog.WriteEntry(this.GetType().Name, $"Controller connected");
					}

					_controller.Update();
					byte[] buffer = _controller.GetBytes();
					_comPort.Write(buffer, 0, buffer.Length);
				}
				else
				{
					if (_controllerIsConnected)
					{
						_controllerIsConnected = false;
						EventLog.WriteEntry(this.GetType().Name, $"Controller disconnected");
					}
				}

				Thread.Sleep(10);
			}
		}

		private void ComDataReceived(object sender, SerialDataReceivedEventArgs e)
		{
			// Do nothing for now. Don't know what the drone is going to return
		}
	}
}