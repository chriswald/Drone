using System.Collections.Generic;
using System.Web.Http;

namespace Monitor.Controllers
{
	[RoutePrefix("api/ComPort")]
    public class ComPortController : ApiController
    {
		[HttpGet]
		[Route("")]
		public IEnumerable<string> GetPortNumbers()
		{
			foreach (string port in System.IO.Ports.SerialPort.GetPortNames())
			{
				yield return port.Substring("COM".Length);
			}
		}
    }
}
