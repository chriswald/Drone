using System;
using System.Collections.Generic;
using SharpDX.XInput;

namespace DroneControllerHardware
{
	public class Point
	{
		public float X { get; internal set; }
		public float Y { get; internal set; }
	}

	public class DroneController
	{
		public const int Deadband = 2500;

		public const byte PacketSize =
			1 + 1 +             // Byte count
			sizeof(float) + 1 + // Left Stick X
			sizeof(float) + 1 + // Left Stick Y
			sizeof(float) + 1 + // Right Stick X
			sizeof(float) + 1 + // Right Stick Y
			sizeof(byte) + 1 +  // Left trigger
			sizeof(byte) + 1 +  // Right trigger
			sizeof(Int32) + 1 + // Button flags
			sizeof(byte) + 1;   // Battery level

		private Controller controller;
		private Gamepad gamepad;
		
		public bool IsConnected
		{
			get
			{
				return controller.IsConnected;
			}
		}

		public BatteryLevel BatteryLevel
		{
			get
			{
				if (controller.IsConnected)
				{
					return controller.GetBatteryInformation(BatteryDeviceType.Gamepad).BatteryLevel;
				}
				else
				{
					return BatteryLevel.Empty;
				}
			}
		}

		public int FakeBatteryPercent
		{
			get
			{
				switch (BatteryLevel)
				{
					case BatteryLevel.Empty: return 0;
					case BatteryLevel.Low: return 5;
					case BatteryLevel.Medium: return 50;
					default: return 100;
				}
			}
		}

		public Point LeftStick
		{
			get
			{
				return new Point()
				{
					X = (Math.Abs((float)gamepad.LeftThumbX) < Deadband) ? 0 : (float)gamepad.LeftThumbX / short.MinValue * -100,
					Y = (Math.Abs((float)gamepad.LeftThumbY) < Deadband) ? 0 : (float)gamepad.LeftThumbY / short.MaxValue * 100,
				};
			}
		}

		public Point RightStick
		{
			get
			{
				return new Point()
				{
					X = (Math.Abs((float)gamepad.RightThumbX) < Deadband) ? 0 : (float)gamepad.RightThumbX / short.MaxValue * 100,
					Y = (Math.Abs((float)gamepad.RightThumbY) < Deadband) ? 0 : (float)gamepad.RightThumbY / short.MaxValue * 100
				};
			}
		}

		public byte LeftTrigger
		{
			get
			{
				return gamepad.LeftTrigger;
			}
		}

		public byte RightTrigger
		{
			get
			{
				return gamepad.RightTrigger;
			}
		}

		public GamepadButtonFlags Buttons
		{
			get
			{
				return gamepad.Buttons;
			}
		}

		public DroneController()
		{
			controller = new Controller(UserIndex.One);
		}

		public void Update()
		{
			if (!IsConnected)
			{
				return;
			}

			gamepad = controller.GetState().Gamepad;
		}

		public byte[] GetBytes()
		{	
			byte separator = 5;

			byte[] buffer = new byte[PacketSize];

			List<byte[]> fields = new List<byte[]>();
			fields.Add(new[] { (byte)PacketSize });
			fields.Add(GetBytes(this.LeftStick.X));
			fields.Add(GetBytes(this.LeftStick.Y));
			fields.Add(GetBytes(this.RightStick.X));
			fields.Add(GetBytes(this.RightStick.Y));
			fields.Add(new[] { this.LeftTrigger });
			fields.Add(new[] { this.RightTrigger });
			fields.Add(GetBytes((int)this.Buttons));
			fields.Add(new[] { (byte)this.BatteryLevel });

			int position = 0;
			foreach (byte[] bytes in fields)
			{
				int fieldLen = bytes.Length;
				Array.Copy(bytes, 0, buffer, position, fieldLen);
				position += fieldLen;
				buffer[position++] = separator;
			}

			return buffer;
		}

		private static unsafe byte[] GetBytes(float value)
		{
			byte[] bytes = new byte[sizeof(float)];
			fixed (byte* b = bytes)
				*((int*)b) = *(int*)&value;
			return bytes;
		}

		private static unsafe byte[] GetBytes(int value)
		{
			byte[] bytes = new byte[sizeof(int)];
			fixed (byte* b = bytes)
				*((int*)b) = *(int*)&value;
			return bytes;
		}
	}
}
