//
//  server.cc
//
//  Copyright (c) 2019 Yuji Hirose. All rights reserved.
//  MIT License
//
#include <chrono>
#include <cstdio>
#include "thirdpart/httplib.hpp"

#define SERVER_CERT_FILE "./cert.pem"
#define SERVER_PRIVATE_KEY_FILE "./key.pem"

using namespace httplib;

std::string dump_headers(const Headers &headers) {
  std::string s;
  char buf[BUFSIZ];

  for (auto it = headers.begin(); it != headers.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%s: %s\n", x.first.c_str(), x.second.c_str());
    s += buf;
  }

  return s;
}

std::string log(const Request &req, const Response &res) {
  std::string s;
  char buf[BUFSIZ];

  s += "\n================================\n";

  snprintf(buf, sizeof(buf), "%s %s %s", req.method.c_str(),
           req.version.c_str(), req.path.c_str());
  s += buf;

  std::string query;
  for (auto it = req.params.begin(); it != req.params.end(); ++it) {
    const auto &x = *it;
    snprintf(buf, sizeof(buf), "%c%s=%s",
             (it == req.params.begin()) ? '?' : '&', x.first.c_str(),
             x.second.c_str());
    query += buf;
  }
  snprintf(buf, sizeof(buf), "%s\n", query.c_str());
  s += buf;

  s += dump_headers(req.headers);

  s += "--------------------------------\n";

  snprintf(buf, sizeof(buf), "%d %s\n", res.status, res.version.c_str());
  s += buf;
  s += dump_headers(res.headers);
  s += "\n";

  if (!res.body.empty()) { s += res.body; }

  s += "\n";

  return s;
}

extern void onrequest(const std::string& msg, const std::string& body, std::string& out);
int servermain(int num) {
#ifdef CPPHTTPLIB_OPENSSL_SUPPORT
  SSLServer svr(SERVER_CERT_FILE, SERVER_PRIVATE_KEY_FILE);
#else
  Server svr;
#endif

  if (!svr.is_valid()) {
    printf("server has an error...\n");
    return -1;
  }

  svr.Get("/", [=](const Request & /*req*/, Response &res) {
    res.set_redirect("/hi");
  });

  svr.Get("/hi", [](const Request & req, Response &res) {
      std::string response;
      onrequest("hi", req.body, response);
      res.set_content(response, "text/plain");
  });
  svr.Post("/cmd", [](const Request& req, Response& res) {
      std::string response;
      
      onrequest("cmd", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/cmd2d", [](const Request& req, Response& res) {
      std::string response;

      onrequest("cmd2d", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/ui", [](const Request& req, Response& res) {
      std::string response;

      onrequest("ui", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/lua", [](const Request& req, Response& res) {
      std::string response;

      onrequest("lua", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Post("/phg", [](const Request& req, Response& res) {
      std::string response;

      onrequest("phg", req.body, response);
      res.set_content(response, "text/plain");
      });
  svr.Get("/slow", [](const Request & /*req*/, Response &res) {
    std::this_thread::sleep_for(std::chrono::seconds(2));
    res.set_content("Slow...\n", "text/plain");
  });

  svr.Get("/dump", [](const Request &req, Response &res) {
    res.set_content(dump_headers(req.headers), "text/plain");
  });

  svr.Get("/stop",
          [&](const Request & /*req*/, Response & /*res*/) { svr.stop(); });

  svr.set_error_handler([](const Request & /*req*/, Response &res) {
    const char *fmt = "<p>Error Status: <span style='color:red;'>%d</span></p>";
    char buf[BUFSIZ];
    snprintf(buf, sizeof(buf), fmt, res.status);
    res.set_content(buf, "text/html");
  });

  svr.set_logger([](const Request &req, const Response &res) {
    printf("%s", log(req, res).c_str());
  });

  //// send message
  //httplib::Client client("127.0.0.1", 8080);
  //client.Post("/ui", "btn \'net test\'", "text/plain");

  // listen
  svr.listen("127.0.0.1", 8081);

  return 0;
}
void sendmsg(const std::string& msg)
{
    // send message
    httplib::Client client("127.0.0.1", 8080);
    client.Post("/ui", msg, "text/plain");

    printf((msg+ "\n").c_str());
}
void sendmsg_2d(const std::string& msg)
{
    // send message
    httplib::Client client("127.0.0.1", 8080);
    client.Post("/cmd2d", msg, "text/plain");

    printf((msg + "\n").c_str());
}