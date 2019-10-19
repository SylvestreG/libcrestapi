//
// Created by syl on 10/17/19.
//

#include <unistd.h>
#include "centreon-api.h"

int main(int ac, char **av) {
  crestapi::api & api{crestapi::api::instance()};
  api.fetch("http://localhost/centreon/api/beta", "admin", "centreon");
}