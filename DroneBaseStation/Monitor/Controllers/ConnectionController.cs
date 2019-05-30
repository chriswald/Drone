using System.Web.Http;
using DroneControllerHardware;

namespace Monitor.Controllers
{
	[RoutePrefix("api/Connection")]
	public class ConnectionController : ApiController
    {
		[HttpGet]
		[Route("Controller")]
		public bool GetControllerConnectedStatus()
		{
			DroneController dc = new DroneController();
			dc.Update();
			return dc.IsConnected;
		}

		[HttpGet]
		[Route("Laptop")]
		public bool GetLaptopConnectedStatus()
		{
			// The laptop's always connected, duh
			return true;
		}

		[HttpGet]
		[Route("Radio")]
		public bool GetRadioRelayConnectedStatus()
		{
			ServiceController serviceController = new ServiceController();
			return serviceController.IsRunning();
		}

		[HttpGet]
		[Route("DroneSystem")]
		public bool GetDroneSystemConnectedStatus()
		{
			return false;
		}

		[HttpGet]
		[Route("DroneMotors")]
		public bool GetDroneMotorsConnectedStatus()
		{
			return false;
		}
	}
}
