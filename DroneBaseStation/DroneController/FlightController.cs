namespace DroneControllerHardware
{
	public class FlightController : DroneController
	{
		public byte Throttle
		{
			get { return this.RightTrigger; }
		}

		public float Roll
		{
			get { return this.LeftStick.X; }
		}

		public float Pitch
		{
			get { return this.LeftStick.Y; }
		}
	}
}
