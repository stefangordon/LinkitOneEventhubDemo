using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Web.Http;

namespace L1Demo
{
    public static class WebApiConfig
    {
        public static void Register(HttpConfiguration config)
        {
            // Web API configuration and services

            // Web API routes
            config.MapHttpAttributeRoutes();

            config.Routes.MapHttpRoute(
                "CrossDomain", "crossdomain.xml",
                new { controller = "CrossDomain" });

            config.Routes.MapHttpRoute(
                name: "DefaultApi",
                defaults: new { controller = "Data" },
                routeTemplate: "data/{deviceId}"
            );

            config.Routes.IgnoreRoute("htm", "*.htm");
            config.Routes.IgnoreRoute("js", "*.js");
            config.Routes.IgnoreRoute("root", "");

        }
    }
}
