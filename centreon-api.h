//
// Created by syl on 10/18/19.
//

#ifndef LIBCRESTAPI__CENTREON_API_H_
#define LIBCRESTAPI__CENTREON_API_H_

#include <stdint.h>
#include <unordered_map>

namespace RestClient {
  class Connection;
};

namespace crestapi {
// A hash function used to hash a pair of any kind
struct hash_pair {
  template<class T1, class T2>
  size_t operator()(const std::pair<T1, T2>& p) const {
    auto hash1 = std::hash<T1>{}(p.first);
    auto hash2 = std::hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};

typedef std::string hostname;
typedef std::pair<std::string, std::string> servicename;

typedef std::string hostgroupname;
typedef std::string servicegroupname;

typedef std::unordered_map<hostname, std::string> hosts;
typedef std::unordered_map<servicename, std::string, hash_pair> services;
typedef std::unordered_map<hostgroupname, hosts> hostgroups;
typedef std::unordered_map<servicegroupname, services> servicegroups;



class api {
 private:
  api();
  void fill_hosts(RestClient::Connection& con, uint32_t page = 1);
  void fill_services(RestClient::Connection& con, uint32_t page = 1);
  void fill_hostgroups(RestClient::Connection& con, uint32_t page = 1);
  void fill_servicegroups(RestClient::Connection& con, uint32_t page = 1);


  std::string _login;
  std::string _pwd;
  hosts _hosts;
  services _services;
  hostgroups _hostgroups;
  servicegroups _servicegroups;

 public:
  static api& instance();

  bool fetch(
    std::string const& url,
    std::string const& login,
    std::string const& pass
  );
  hosts const& get_hosts(void) const;
  services const& get_services(void) const;
  hostgroups const& get_hostgroups(void) const;
  servicegroups const& get_servicegroups(void) const;
};
};

#endif //LIBCRESTAPI__CENTREON_API_H_
