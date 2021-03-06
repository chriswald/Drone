﻿using Monitor.Service;
using System;
using System.Runtime.Caching;
using System.ServiceProcess;
using System.Web.Http;
using static Monitor.Service.RadioRelayService;

namespace Monitor.Controllers
{
	[RoutePrefix("api/Service")]
	public class ServiceController : ApiController
	{
		private const string ServiceCacheName = nameof(RadioRelayService);


		[HttpGet]
		[Route(nameof(Status))]
		public ServiceControllerStatus Status()
		{
			if (MemoryCache.Default.Contains(ServiceCacheName))
			{
				return ServiceControllerStatus.Running;
			}
			else
			{
				return ServiceControllerStatus.Stopped;
			}
		}
		
		[HttpGet]
		[Route(nameof(Start) + "/{comId:int}")]
		public void Start(int comId)
		{
			if (!MemoryCache.Default.Contains(ServiceCacheName))
			{
				try
				{
					RadioRelayService service = new RadioRelayService(comId);
					service.Start();
					MemoryCache.Default.Add(ServiceCacheName, service, DateTime.Now.AddYears(1));
				}
				catch { }
			}
		}
		
		[HttpGet]
		[Route(nameof(Stop))]
		public void Stop()
		{
			if (MemoryCache.Default.Contains(ServiceCacheName))
			{
				RadioRelayService service = (RadioRelayService)MemoryCache.Default.Get(ServiceCacheName);
				service.Stop();
				MemoryCache.Default.Remove(ServiceCacheName);
			}
		}

		[HttpGet]
		[Route(nameof(IsRunning))]
		public bool IsRunning()
		{
			if (MemoryCache.Default.Contains(ServiceCacheName))
			{
				// Make this more robust to send a message to the Teensy to ask if it's running.
				RadioRelayService service = (RadioRelayService)MemoryCache.Default.Get(ServiceCacheName);
				return service.IsRelayConnected();
			}
			else
			{
				return false;
			}
		}

		[HttpGet]
		[Route(nameof(BatteryLevel))]
		public int BatteryLevel([FromBody] DroneBattery battery)
		{
			if (MemoryCache.Default.Contains(ServiceCacheName))
			{
				RadioRelayService service = (RadioRelayService)MemoryCache.Default.Get(ServiceCacheName);
				return service.GetBatteryLevel(battery);
			}
			else
			{
				return 0;
			}
		}
	}
}