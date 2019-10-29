//
// Created by syl on 10/17/19.
//

#include <unistd.h>
#include <iostream>
#include "centreon-api.h"

int main(int ac, char **av) {
  crestapi::api & api{crestapi::api::instance()};
  api.fetch("http://localhost/centreon/api/beta", "admin", "centreon");

  std::cout << "hosts \t\t: " << api.get_hosts().size() << std::endl;
  std::cout << "services \t: " << api.get_hosts().size() << std::endl;
  std::cout << "hostgroups \t: " << api.get_hostgroups().size() << std::endl;
  std::cout << "servicegroups \t: " << api.get_hostgroups().size() << std::endl;
}