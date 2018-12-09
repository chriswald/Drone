using Microsoft.Web.Administration;
using System.Web.Hosting;
using System.Web.Http;

namespace Monitor.Controllers
{
	[RoutePrefix("api/AppInsights")]
    public class AppInsightsController : ApiController
    {
		[Route("AppName")]
		public string GetApplicationName()
		{
			ServerManager manager = new ServerManager();
			string applicationAlias = HostingEnvironment.ApplicationVirtualPath;
			string applicationName = applicationAlias.Substring(1);
			return applicationName;
		}
    }
}
