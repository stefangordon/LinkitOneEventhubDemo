using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Web;
using System.Web.Http;
using System.Web.Routing;
using Microsoft.AspNet.SignalR;
using Microsoft.ServiceBus.Messaging;

namespace L1Demo
{
    public class WebApiApplication : System.Web.HttpApplication
    {
        protected void Application_Start()
        {
            GlobalConfiguration.Configure(WebApiConfig.Register);

            // Create the EventHubClient
            EventHubClient eventHubClient = EventHubClient.Create("hub");

            // Get the default Subscriber Group
            EventHubConsumerGroup defaultConsumerGroup = eventHubClient.GetDefaultConsumerGroup();

            // Grab the SignalR hub
            var hubContext = GlobalHost.ConnectionManager.GetHubContext<ViewHub>();

            // Figure out the number of partitions
            var runtimeInfo = eventHubClient.GetRuntimeInformation();
            var partitions = runtimeInfo.PartitionIds;

            foreach(string partition in partitions)
            {
                Task.Factory.StartNew(() =>
                {
                    Debug.WriteLine(partition);
                    EventHubReceiver consumer = defaultConsumerGroup.CreateReceiver(partition, DateTime.UtcNow);

                    while (true)
                    {
                        Thread.Sleep(500);
                        try
                        {
                            var eventData = consumer.Receive();

                            if (eventData != null)
                            {
                                hubContext.Clients.All.Update(Encoding.UTF8.GetString(eventData.GetBytes()));
                            }
                        }
                        catch (Exception ex)
                        {  
                            Thread.Sleep(1000);
                        }
                    }
                });
            }
           
        }
    }
}
