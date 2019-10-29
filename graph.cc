//
// Created by syl on 10/17/19.
//

#include <unistd.h>
#include <fstream>
#include <regex>
#include "centreon-api.h"

int main(int ac, char **av) {
  crestapi::api & api{crestapi::api::instance()};
  api.fetch("http://localhost/centreon/api/beta", "admin", "centreon");

  std::ofstream ofs;

  ofs.open("output.csv");
  ofs << "size,path" << std::endl;
  ofs << ";poller1" << std::endl;

  for (auto const& h : api.get_hosts()) {
    ofs << "200;poller1/" << h.first << std::endl;
  }

  for (auto const& s : api.get_services()) {
    std::string str = s.first.second;
    str = std::regex_replace(str, std::regex("/"), "-");
    ofs << "400;poller1/" << s.first.first << "/" << str << std::endl;
  }

}