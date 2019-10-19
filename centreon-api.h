//
// Created by syl on 10/18/19.
//

#ifndef LIBCRESTAPI__CENTREON_API_H_
#define LIBCRESTAPI__CENTREON_API_H_

#include <stdint.h>
#include <unordered_map>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>

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

typedef uint64_t hostid;
typedef std::pair<uint64_t, uint64_t> serviceid;

typedef std::string hostgroupname;
typedef std::string servicegroupname;

typedef std::unordered_map<hostname, hostid> hosts;
typedef std::unordered_map<servicename, serviceid, hash_pair> services;
typedef std::unordered_map<hostgroupname, hosts> hostgroups;
typedef std::unordered_map<servicegroupname, services> servicegroups;

typedef std::unordered_map<hostid, hostname> hosts_by_ids;
typedef std::unordered_map<serviceid, servicename, hash_pair> services_by_ids;



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
  hosts_by_ids _hosts_by_ids;
  services_by_ids _services_by_ids;


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
  hosts_by_ids const& get_hosts_by_ids(void) const;
  services_by_ids const& get_services_by_ids(void) const;
};
};

#endif //LIBCRESTAPI__CENTREON_API_H_
