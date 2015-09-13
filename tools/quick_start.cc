#include <exception>
#include <iostream>

#include <geo_base/geo_base.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "USAGE: quick-start <geodata.dat>";
    return -1;
  }

  geo_base::GeoBasePtr geo_base = NULL;
  try {
    geo_base = geo_base::GeoBasePtr(new geo_base::GeoBase(argv[1]));
  } catch (const std::exception& e) {
    std::cerr << "EXCEPTION: " << e.what() << std::endl;
    return -1;
  }

  geo_base::Location location;
  while (std::cin >> location) {
    geo_base::RegionID region_id = geo_base->Lookup(location);
    if (region_id != geo_base::kUnknownRegionID) {
      std::cerr << region_id << std::endl;
    } else {
      std::cerr << -1 << std::endl;
    }
  }

  return 0;
}
