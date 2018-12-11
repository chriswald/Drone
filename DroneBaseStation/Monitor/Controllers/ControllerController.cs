using DroneControllerHardware;
using System;
using System.Net;
using System.Net.Http;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using System.Web.WebSockets;

namespace Monitor.Controllers
{
	[RoutePrefix("api/Controller")]
    public class ControllerController : ApiController
    {
		[Route("")]
		public HttpResponseMessage Get()
		{
			HttpContext context = HttpContext.Current;

			if (context.IsWebSocketRequest || context.IsWebSocketRequestUpgrading)
			{
				context.AcceptWebSocketRequest(ProcessWSDataRequest);
				return Request.CreateResponse(HttpStatusCode.SwitchingProtocols);
			}

			return Request.CreateResponse(HttpStatusCode.BadRequest);
		}

		private async Task ProcessWSDataRequest(AspNetWebSocketContext context)
		{
			WebSocket socket = context.WebSocket;
			CancellationTokenSource cts = new CancellationTokenSource();
			
			StreamDataToClient(socket, cts.Token);

			ArraySegment<byte> buffer = new ArraySegment<byte>(new byte[1024 * 4]);
			WebSocketReceiveResult result;

			do
			{
				result = await socket.ReceiveAsync(buffer, CancellationToken.None);
			} while (!result.CloseStatus.HasValue);

			cts.Cancel();
			await socket.CloseAsync(result.CloseStatus.Value, result.CloseStatusDescription, CancellationToken.None);
		}

		private void StreamDataToClient(WebSocket socket, CancellationToken cancellationToken)
		{
			Task.Run(async () =>
			{
				while (!cancellationToken.IsCancellationRequested)
				{
					DroneController droneController = new DroneController();
					droneController.Update();

					if (droneController.IsConnected)
					{
						await socket.SendAsync(new ArraySegment<byte>(droneController.GetBytes()), WebSocketMessageType.Binary, true, CancellationToken.None);
						Thread.Sleep(10);
					}
					else
					{
						Thread.Sleep(100);
					}
				}
			});
		}
	}
}
