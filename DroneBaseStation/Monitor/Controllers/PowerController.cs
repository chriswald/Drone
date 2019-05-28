using System;
using System.IO;
using System.IO.Pipes;
using System.Management;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Formatters.Binary;
using System.Web.Http;
using DroneControllerHardware;
using Monitor.Service;

namespace Monitor.Controllers
{
	[RoutePrefix("api/Power")]
    public class PowerController : ApiController
    {
		private enum BatteryLevel
		{
			Empty = 0,
			Low = 5,
			Medium = 50,
			Full = 100
		}

		[HttpGet]
		[Route("Controller")]
		public int GetControllerBatteryFakePercent()
		{
			try
			{
				DroneController dc = new DroneController();
				dc.Update();
				return dc.FakeBatteryPercent;
			}
			catch { }

			return 0;
		}

		[HttpGet]
		[Route("Laptop")]
		public int GetLaptopBatteryPercent()
		{
			ObjectQuery query = new ObjectQuery("Select EstimatedChargeRemaining FROM Win32_Battery");
			ManagementObjectSearcher searcher = new ManagementObjectSearcher(query);
			ManagementObjectCollection collection = searcher.Get();

			foreach(ManagementObject obj in collection)
			{
				foreach (PropertyData data in obj.Properties)
				{
					return int.Parse(data.Value.ToString());
				}
			}

			return 0;
		}

		[HttpGet]
		[Route("Radio")]
		public int GetRadioRelayBatteryPercent()
		{
			// The radio draws power from the laptop and doesn't have
			// a battery of its own so report full when it's connected
			// and empty when it's not.

			ConnectionController connectionController = new ConnectionController();

			if (connectionController.GetRadioRelayConnectedStatus() == true)
			{
				return 100;
			}
			else
			{
				return 0;
			}
		}

		[HttpGet]
		[Route("Drone")]
		public int GetDroneBatteryPercent()
		{
			try
			{
				ServiceController serviceController = new ServiceController();
				return serviceController.BatteryLevel(RadioRelayService.DroneBattery.System);
			}
			catch { }

			return 0;
		}
	}
}
