using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Http;
using System.Text;
using System.Threading.Tasks;
using System.Web.Http;

namespace L1Demo.Controllers
{
    public class DataController : ApiController
    {        
        private readonly string HubUrl = "{0}/publishers/{1}/messages";

        public async Task<HttpResponseMessage> Post([FromBody] string eventData, string deviceId)
        {
            string eventHubURL = System.Configuration.ConfigurationManager.AppSettings["EventHubUrl"];
            string populatedUrl = string.Format(HubUrl, eventHubURL, deviceId);

            var request = new HttpRequestMessage();
            request.Method = HttpMethod.Post;
            request.RequestUri = new Uri(populatedUrl);
            request.Content = new StringContent(eventData, Encoding.UTF8, "application/atom+xml");

            request.Headers.Add("Authorization", this.Request.Headers.GetValues("Authorization").First());            

            HttpClient client = new HttpClient();
            var response = await client.SendAsync(request, HttpCompletionOption.ResponseHeadersRead);
            return response;
        }
    }
}
