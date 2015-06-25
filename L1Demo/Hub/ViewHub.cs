using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using Microsoft.AspNet.SignalR;

namespace L1Demo
{
    public class ViewHub : Hub
    {
        public void Update(string data)
        {
            // Call the broadcastMessage method to update clients.
            Clients.All.broadcastMessage(data);
        }
    }
}