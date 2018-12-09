using System;
using System.Diagnostics;
using System.IO.Ports;
using System.Threading;
using System.Threading.Tasks;
using DroneControllerHardware;

namespace Monitor.Service
{
	public class RadioRelayService
	{
		private delegate void ConnectionMessageReceivedDelegate();

		private SerialPort _comPort;

		private DroneController _controller;
		private bool _controllerIsConnected;

		private volatile bool _serviceDone;

		private Thread _workerThread;

		private event ConnectionMessageReceivedDelegate ConnectionMessageReceived;

		public bool IsRunning { get; private set; }

		public RadioRelayService(int comPortId)
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

		public bool IsRelayConnected()
		{
			byte[] buffer = new[] { Convert.ToByte('C') };
			bool isConnected = false;

			ConnectionMessageReceivedDelegate callback = () =>
			{
				isConnected = true;
			};

			this.ConnectionMessageReceived += callback;

			Task task = Task.Run(() =>
			{
				while (!isConnected) { Thread.Sleep(10); }
			});

			_comPort.Write(buffer, 0, buffer.Length);

			if (Task.WhenAny(task, Task.Delay(500)).Result == task)
			{
				this.ConnectionMessageReceived -= callback;
				return true;
			}
			else
			{
				this.ConnectionMessageReceived -= callback;
				return false;
			}
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
			while(_comPort.IsOpen && _comPort.BytesToRead > 0)
			{
				string line = _comPort.ReadLine().TrimEnd();

				if (line == "Connected")
				{
					OnConnectionMessageReceived();
				}
			}
		}

		private void OnConnectionMessageReceived()
		{
			ConnectionMessageReceived?.Invoke();
		}
	}
}