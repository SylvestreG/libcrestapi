//
// Created by syl on 10/18/19.
//

#include <json11.hpp>
#include <sstream>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include "centreon-api.h"

using namespace crestapi;

api::api() {

};

api&
api::instance() {
  static api api;
  return api;
}

hosts const&
api::get_hosts(void) const {
  return _hosts;
}

services const&
api::get_services(void) const {
  return _services;
}

hostgroups const&
api::get_hostgroups(void) const {
  return _hostgroups;
}
servicegroups const&
api::get_servicegroups(void) const {
  return _servicegroups;
}

hosts_by_ids const&
api::get_hosts_by_ids(void) const {
  return _hosts_by_ids;
}

services_by_ids const&
api::get_services_by_ids(void) const {
  return _services_by_ids;
}

bool api::fetch(
  std::string const& url,
  std::string const& login,
  std::string const& pass) {

  RestClient::init();

  RestClient::HeaderFields headers;
  headers["Accept"] = "application/json";

  RestClient::Connection *conn = new RestClient::Connection(url);
  conn->SetUserAgent("pooay");
  conn->FollowRedirects(true);
  conn->FollowRedirects(true, 3);
  conn->SetHeaders(headers);

  std::string token;
  {
    json11::Json jsq{
      json11::Json::object{
        {
          "security", json11::Json::object{
          {
            "credentials", json11::Json::object{
            {"login", login},
            {"password", pass}
          }
          }
        }
        }
      }
    };
    conn->AppendHeader("Content-Type", "application/json");
    RestClient::Response r = conn->post("/login", jsq.dump());

    std::string err;
    json11::Json js{json11::Json::parse(r.body, err)};
    if (!err.empty())
      return false;

    if (js.is_object() && js["security"].is_object()
      && js["security"]["token"].is_string())
      token = js["security"]["token"].string_value();
    headers["X-AUTH-TOKEN"] = token;
    conn->SetHeaders(headers);
  }

  fill_hosts(*conn);
  fill_services(*conn);
  fill_hostgroups(*conn);
  fill_servicegroups(*conn);
  return true;
}

void api::fill_hosts(RestClient::Connection& conn, uint32_t page) {
  std::stringstream ss;
  uint32_t limit{100};
  ss << "/monitoring/hosts?limit=" << limit << "&" << "page=" << page;
  RestClient::Response r = conn.get(ss.str());

  std::string err;
  json11::Json js{json11::Json::parse(r.body, err)};
  if (!err.empty())
    return;

  for (auto it : js["result"].array_items()) {
    _hosts[it["name"].string_value()] = it["id"].int_value();
  }

  uint32_t total = js["meta"]["total"].int_value();
  uint32_t max_page = total / limit;
  ((total % max_page) != 0) ? max_page++ : max_page;

  if (page != max_page) {
    fill_hosts(conn, ++page);
  }
}

void api::fill_services(RestClient::Connection& conn, uint32_t page) {
  std::stringstream ss;
  uint32_t limit{100};
  ss << "/monitoring/services?limit=" << limit << "&" << "page=" << page;
  RestClient::Response r = conn.get(ss.str());

  std::string err;
  json11::Json js{json11::Json::parse(r.body, err)};
  if (!err.empty())
    return;

  for (auto it : js["result"].array_items()) {
    _services[{it["host"]["name"].string_value(),
               it["display_name"].string_value()}] =
      {it["host"]["id"].int_value(), it["id"].int_value()};
  }

  uint32_t total = js["meta"]["total"].int_value();
  uint32_t max_page = total / limit;
  ((total % max_page) != 0) ? max_page++ : max_page;

  if (page != max_page) {
    fill_services(conn, ++page);
  }
}

void api::fill_hostgroups(RestClient::Connection& conn, uint32_t page) {
  std::stringstream ss;
  uint32_t limit{100};
  ss << "/monitoring/hostgroups?show_host=true&limit=" << limit << "&"
     << "page=" << page;
  RestClient::Response r = conn.get(ss.str());

  std::string err;
  json11::Json js{json11::Json::parse(r.body, err)};
  if (!err.empty())
    return;

  for (auto it : js["result"].array_items()) {
    for (auto h : it["hosts"].array_items())
      _hostgroups[it["name"].string_value()][h["name"].string_value()] =
        h["id"].int_value();
  }

  uint32_t total = js["meta"]["total"].int_value();
  uint32_t max_page = total / limit;
  if (max_page == 0)
    max_page = page;
  ((total % max_page) != 0) ? max_page++ : max_page;

  if (page != max_page) {
    fill_hostgroups(conn, ++page);
  }
}

void api::fill_servicegroups(RestClient::Connection& conn, uint32_t page) {
  std::stringstream ss;
  uint32_t limit{100};
  ss << "/monitoring/servicegroups?show_service=true&limit=" << limit << "&"
     << "page=" << page;
  RestClient::Response r = conn.get(ss.str());

  std::string err;
  json11::Json js{json11::Json::parse(r.body, err)};
  if (!err.empty())
    return;

  for (auto it : js["result"].array_items()) {
    for (int i = 0; i < it["hosts"].array_items().size(); i++) {
      _servicegroups[it["name"].string_value()][{
        it["hosts"][i]["name"].string_value(),
        it["services"][i]["name"].string_value()}] =
        {it["hosts"][i]["id"].int_value(), it["services"][i]["id"].int_value()};
    }
  }

  uint32_t total = js["meta"]["total"].int_value();
  uint32_t max_page = total / limit;
  if (max_page == 0)
    max_page = page;
  ((total % max_page) != 0) ? max_page++ : max_page;

  if (page != max_page) {
    fill_servicegroups(conn, ++page);
  }
}