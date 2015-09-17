// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
//   The above copyright notice and this permission notice shall be included
//   in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <algorithm>
#include <cstdint>
#include <iomanip>
#include <mutex>
#include <stack>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <netinet/in.h>

#include "file.h"
#include "io.h"
#include "location.h"
#include "log.h"
#include "stop_watch.h"
#include "util.h"
#include "zlib_wrapper.h"
#include "get_opt.h"

#include "proto/fileformat.pb.h"
#include "proto/osmformat.pb.h"
#include "proto/region.pb.h"

using namespace geo_base;

typedef RegionID OSMID;

struct KeyValue {
  const char* key;
  const char* value;
};

struct Reference {
  OSMPBF::Relation::MemberType member_type;
  OSMID osm_id;
  const char* role;

  Reference(const OSMPBF::Relation::MemberType& member_type, OSMID osm_id,
            const std::string& role) :
      member_type(member_type),
      osm_id(osm_id),
      role(role.c_str()) {
  }
};

class ProtobufReader {
 public:
  ProtobufReader(const char* path) {
    file.ReadOnlyOpen(path);
  }

  bool ReadNext(OSMPBF::BlobHeader* header, OSMPBF::Blob* blob,
      std::vector<char>& buffer) const {
    std::lock_guard<std::mutex> lock(mutex);

    int32_t header_size = 0;
    if (!Read(file.fd(), (char*) &header_size, sizeof(header_size)))
      return false;

    header_size = ntohl(header_size);

    buffer.resize(header_size);
    if (!Read(file.fd(), buffer.data(), buffer.size())) {
      LogError("geo-base-convert") << "Can't read header";
      return false;
    }

    if (!header->ParseFromArray(buffer.data(), buffer.size())) {
      LogError("geo-base-convert") << "Can't parse header";
      return false;
    }

    int32_t blob_size = header->datasize();

    buffer.resize(blob_size);
    if (!Read(file.fd(), buffer.data(), buffer.size())) {
      LogError("geo-base-convert") << "Can't read blob";
      return false;
    }

    if (!blob->ParseFromArray(buffer.data(), buffer.size())) {
      LogError("geo-base-convert") << "Can't parse blob";
      return false;
    }

    return true;
  }

 private:
  File file;
  mutable std::mutex mutex;
};

template<typename Callback>
class ProtobufParser {
 public:
  ProtobufParser(const ProtobufReader& reader, Callback &callback) :
      reader(&reader),
      callback(&callback) {
  }

  void operator () () {
    OSMPBF::BlobHeader header;
    OSMPBF::Blob blob;
    OSMPBF::PrimitiveBlock block;
    OSMPBF::PrimitiveGroup group;

    while (reader->ReadNext(&header, &blob, buffer)) {
      if (header.type() == "OSMHeader")
        continue;

      if (header.type() != "OSMData")
        throw Exception("Unknown header type: %s", header.type().c_str());

      if (blob.has_raw())
        throw Exception("%s", "Can't parse raw data");

      if (blob.has_lzma_data())
        throw Exception("%s", "Can't parse lzma data");

      if (!ParseBlock(blob, &block))
        throw Exception("%s", "Can't parse block");

      if (!ProcessGroups(block))
        throw Exception("%s", "Can't process group");
    }
  }

 private:
  template<typename T>
  const std::vector<KeyValue>& GetTags(const T& obj,
      const OSMPBF::PrimitiveBlock& block) {
    kvs.clear();
    for (int i = 0; i < obj.keys_size(); ++i) {
      uint64_t k = obj.keys(i);
      uint64_t v = obj.vals(i);

      KeyValue kv;
      kv.key = block.stringtable().s(k).c_str();
      kv.value = block.stringtable().s(v).c_str();

      kvs.push_back(kv);
    }
    return kvs;
  }

  bool ParseBlock(const OSMPBF::Blob& blob, OSMPBF::PrimitiveBlock* block) {
    buffer.resize(blob.raw_size());

    ZLibWrapper lib;
    auto const &z = blob.zlib_data();
    lib.Unpack(z.data(), z.size(), buffer);

    if (!block->ParseFromArray(buffer.data(), buffer.size()))
      return false;

    return true;
  }

  bool ProcessGroups(const OSMPBF::PrimitiveBlock& block) {
    for (int k = 0; k < block.primitivegroup_size(); ++k) {
      const OSMPBF::PrimitiveGroup& group = block.primitivegroup(k);

      for (int i = 0; i < group.nodes_size(); ++i) {
        const OSMPBF::Node& node = group.nodes(i);

        Location l;
        l.lon = 1e-9 * (block.lon_offset() + block.granularity() * node.lon());
        l.lat = 1e-9 * (block.lat_offset() + block.granularity() * node.lat());

        callback->NodeCallback(node.id(), l, GetTags(node, block));
      }

      if (group.has_dense()) {
        const OSMPBF::DenseNodes& nodes = group.dense();

        OSMID osm_id = 0;
        Location location;

        int cur_kv = 0;

        for (int i = 0; i < nodes.id_size(); ++i) {
          osm_id += nodes.id(i);
          location.lon += 1e-9 * (block.lon_offset() + block.granularity() * nodes.lon(i));
          location.lat += 1e-9 * (block.lat_offset() + block.granularity() * nodes.lat(i));

          kvs.clear();
          while (cur_kv < nodes.keys_vals_size() && nodes.keys_vals(cur_kv) != 0) {
            uint64_t k = nodes.keys_vals(cur_kv);
            uint64_t v = nodes.keys_vals(cur_kv + 1);

            KeyValue kv;
            kv.key = block.stringtable().s(k).c_str();
            kv.value = block.stringtable().s(v).c_str();

            kvs.push_back(kv);

            cur_kv += 2;
          }
          ++cur_kv;

          callback->NodeCallback(osm_id, location, kvs);
        }
      }

      for (int i = 0; i < group.ways_size(); ++i) {
        const OSMPBF::Way& w = group.ways(i);

        way_refs.clear();
        OSMID ref = 0;
        for (int j = 0; j < w.refs_size(); ++j) {
          ref += w.refs(j);
          way_refs.push_back(ref);
        }

        callback->WayCallback(w.id(), GetTags(w, block), way_refs);
      }

      for (int i = 0; i < group.relations_size(); ++i) {
        const OSMPBF::Relation& r = group.relations(i);

        refs.clear();
        OSMID osm_id = 0;
        for (int j = 0; j < r.memids_size(); ++j) {
          osm_id += r.memids(j);
          refs.push_back(Reference(r.types(j), osm_id, block.stringtable().s(r.roles_sid(j))));
        }

        callback->RelationCallback(r.id(), GetTags(r, block), refs);
      }
    }

    return true;
  }

  const ProtobufReader* reader;
  Callback* callback;
  std::vector<char> buffer;
  std::vector<KeyValue> kvs;
  std::vector<OSMID> way_refs;
  std::vector<Reference> refs;
};

template<typename Callback>
class MTProtobufParser {
 public:
  MTProtobufParser(const std::string& path, std::vector<Callback>& callbacks) :
      reader(path.c_str()),
      pbf_path(path),
      callbacks(callbacks) {
  }

  void operator () () {
    LogInfo("geo-base-convert") << "Parse \"" << pbf_path << "\"...";
    StopWatch stop_watch;

    std::vector<ProtobufParser<Callback>> parsers;
    for (Count i = 0; i < callbacks.size(); ++i)
      parsers.emplace_back(reader, callbacks[i]);

    std::vector<std::thread> threads(parsers.size());
    for (Count i = 0; i < threads.size(); ++i)
      threads[i]= std::thread(std::ref(parsers[i]));

    for (Count i = 0; i < threads.size(); ++i)
      threads[i].join();

    double spent = stop_watch.CheckPoint() / 60.;
    LogInfo("geo-base-convert") << "\"" << pbf_path << "\" parsed by "
        << spent << " MIN";
  }

 private:
  ProtobufReader reader;
  std::string pbf_path;
  std::vector<Callback> &callbacks;
};

class PBFCallback {
 public:
  void NodeCallback(OSMID, const Location&, const std::vector<KeyValue>&) {
  }

  void WayCallback(OSMID, const std::vector<KeyValue>&,
      const std::vector<OSMID>&) {
  }

  void RelationCallback(OSMID, const std::vector<KeyValue>&,
      const std::vector<Reference>&) {
  }
};

static uint64_t GetOptions(const std::vector<KeyValue>& kvs) {
  uint64_t options = 0;
  for (const KeyValue& kv : kvs) {
    if (!strcmp(kv.key, "boundary")) {
      if (!strcmp(kv.value, "administrative"))
        options |= (1ULL << proto::Region::BOUNDARY_ADMINISTRATIVE);
      else if (!strcmp(kv.value, "maritime"))
        options |= (1ULL << proto::Region::BOUNDARY_MARITIME);
      else if (!strcmp(kv.value, "political"))
        options |= (1ULL << proto::Region::BOUNDARY_POLICAL);
      else if (!strcmp(kv.value, "vice_county"))
        options |= (1ULL << proto::Region::BOUNDARY_VICE_COUNTRY);
      else if (!strcmp(kv.value, "national_park"))
        options |= (1ULL << proto::Region::BOUNDARY_NATIONAL_PARK);
      else if (!strcmp(kv.value, "national"))
        options |= (1ULL << proto::Region::BOUNDARY_NATIONAL);
      else if (!strcmp(kv.value, "civil"))
        options |= (1ULL << proto::Region::BOUNDARY_CIVIL);

    } else if (!strcmp(kv.key, "place")) {
      if (!strcmp(kv.value, "allotments"))
        options |= (1ULL << proto::Region::PLACE_ALLOTMENTS);
      else if (!strcmp(kv.value, "city"))
        options |= (1ULL << proto::Region::PLACE_CITY);
      else if (!strcmp(kv.value, "city_block"))
        options |= (1ULL << proto::Region::PLACE_CITY_BLOCK);
      else if (!strcmp(kv.value, "country"))
        options |= (1ULL << proto::Region::PLACE_COUNTRY);
      else if (!strcmp(kv.value, "district"))
        options |= (1ULL << proto::Region::PLACE_DISTRICT);
      else if (!strcmp(kv.value, "farm"))
        options |= (1ULL << proto::Region::PLACE_FARM);
      else if (!strcmp(kv.value, "hamlet"))
        options |= (1ULL << proto::Region::PLACE_HAMLET);
      else if (!strcmp(kv.value, "locality"))
        options |= (1ULL << proto::Region::PLACE_LOCALITY);
      else if (!strcmp(kv.value, "municipality"))
        options |= (1ULL << proto::Region::PLACE_MINUCUPALITY);
      else if (!strcmp(kv.value, "neighbourhood"))
        options |= (1ULL << proto::Region::PLACE_NEIGHBOURHOOD);
      else if (!strcmp(kv.value, "plot"))
        options |= (1ULL << proto::Region::PLACE_PLOT);
      else if (!strcmp(kv.value, "province"))
        options |= (1ULL << proto::Region::PLACE_PROVINCE);
      else if (!strcmp(kv.value, "quarter"))
        options |= (1ULL << proto::Region::PLACE_QUARTER);
      else if (!strcmp(kv.value, "region"))
        options |= (1ULL << proto::Region::PLACE_REGION);
      else if (!strcmp(kv.value, "state"))
        options |= (1ULL << proto::Region::PLACE_STATE);
      else if (!strcmp(kv.value, "suburb"))
        options |= (1ULL << proto::Region::PLACE_SUBURB);
      else if (!strcmp(kv.value, "town"))
        options |= (1ULL << proto::Region::PLACE_TOWN);
      else if (!strcmp(kv.value, "village"))
        options |= (1ULL << proto::Region::PLACE_VILLAGE);
    }
  }
  return options;
}

static bool IsBoundary(const std::vector<KeyValue>& kvs) {
  if (GetOptions(kvs) == 0)
    return false;
  for (const KeyValue& kv : kvs)
    if (strstr(kv.key, "name") == kv.key)
      return true;
  return false;
}

static bool IsWayReference(const Reference& r) {
  if (r.member_type == OSMPBF::Relation::WAY &&
      (!strcmp(r.role, "outer") || !strcmp(r.role, "inner") || !r.role))
    return true;
  return false;
}

struct WaysCallback : public PBFCallback {
  std::unordered_set<OSMID> ways;

  void WayCallback(OSMID osm_id, const std::vector<KeyValue>& kvs,
      const std::vector<OSMID>& refs) {
    if (IsBoundary(kvs) && refs.front() == refs.back())
      ways.insert(osm_id);
  }

  void RelationCallback(OSMID, const std::vector<KeyValue>& kvs,
      const std::vector<Reference>& refs) {
    if (IsBoundary(kvs))
      for (const Reference& r : refs)
        if (IsWayReference(r))
          ways.insert(r.osm_id);
  }
};

struct NodeIDsCallback : public PBFCallback {
  std::unordered_set<OSMID> const *need_ways;

  std::unordered_set<OSMID> nodes;
  std::unordered_map<OSMID, std::vector<OSMID>> ways;

  NodeIDsCallback(const std::unordered_set<OSMID>& w) :
      need_ways(&w) {
  }

  void WayCallback(OSMID osm_id, const std::vector<KeyValue>&,
      const std::vector<OSMID>& n) {
    if (need_ways->find(osm_id) != need_ways->end()) {
      for (OSMID node_id : n) {
        nodes.insert(node_id);
        ways[osm_id].push_back(node_id);
      }
    }
  }
};

struct NodesCallback : public PBFCallback {
  const std::unordered_set<OSMID>* need_nodes;

  std::unordered_map<OSMID, Location> nodes;

  NodesCallback(const std::unordered_set<OSMID>& need_nodes) :
      need_nodes(&need_nodes) {
  }

  void NodeCallback(OSMID osm_id, const Location& location,
      const std::vector<KeyValue>&) {
    if (need_nodes->find(osm_id) != need_nodes->end())
      nodes[osm_id] = location;
  }
};

struct Parser : public PBFCallback {
  typedef std::unordered_map<OSMID, Location> Nodes;
  typedef std::unordered_map<OSMID, std::vector<OSMID>> Ways;

  const Nodes* nodes;
  const Ways* ways;

  proto::Region region;
  std::string buffer;
  ProtoWriter* writer;

  std::unordered_map<OSMID, std::vector<OSMID>> graph;
  std::unordered_set<OSMID> used;
  std::stack<OSMID, std::vector<OSMID>> stack;

  PolygonID polygon_id;
  PolygonID polygon_id_step;

  PolygonID GeneratePolygonID() {
    PolygonID ret = polygon_id;
    polygon_id += polygon_id_step;
    return ret;
  }

  Parser(const Nodes& nodes, const Ways& ways, ProtoWriter& writer,
         PolygonID start, PolygonID step) :
      nodes(&nodes),
      ways(&ways),
      writer(&writer),
      polygon_id(start),
      polygon_id_step(step) {
  }

  void AddLocation(proto::Polygon* p, OSMID osm_id) const {
    proto::Location* location = p->add_locations();
    location->set_lon(nodes->at(osm_id).lon);
    location->set_lat(nodes->at(osm_id).lat);
  }

  void WayCallback(OSMID osm_id, const std::vector<KeyValue>& kvs,
      const std::vector<OSMID>& refs) {
    if (IsBoundary(kvs) && refs.back() == refs.front()) {
      region.Clear();
      region.set_region_id(osm_id);
      region.set_options(GetOptions(kvs));

      proto::Polygon* polygon = region.add_polygons();
      polygon->set_polygon_id(GeneratePolygonID());
      polygon->set_type(proto::Polygon::TYPE_OUTER);
      for (const OSMID& osm_id : refs)
        AddLocation(polygon, osm_id);

      for (const KeyValue& x : kvs) {
        proto::KeyValue* kv = region.add_kvs();
        kv->set_key(x.key);
        kv->set_value(x.value);
      }

      buffer.clear();
      region.SerializeToString(&buffer);

      writer->Write(buffer);
    }
  }

  void SaveLocations(proto::Polygon* polygon, OSMID osm_id) {
    stack.push(osm_id);
    while (!stack.empty()) {
      osm_id = stack.top();
      stack.pop();

      used.insert(osm_id);
      AddLocation(polygon, osm_id);

      for (const OSMID& node_id : graph[osm_id])
        if (used.find(node_id) == used.end())
          stack.push(node_id);
    }
  }


  void RelationCallback(OSMID osm_id, const std::vector<KeyValue>& kvs,
      const std::vector<Reference>& refs) {
    if (IsBoundary(kvs)) {
      graph.clear();
      used.clear();

      bool found = true;
      for (const Reference& r : refs)
        if (IsWayReference(r) && ways->find(r.osm_id) == ways->end())
          found = false;

      if (!found) {
        LogWarning("geo-base-convert", "not found", osm_id)
            << "Keys and values:";

        for (const KeyValue& kv : kvs)
          LogWarning("geo-base-convert", "not found", osm_id) << "  "
              << kv.key << ' ' << kv.value;

        return;
      }

      for (const Reference& r : refs)
        if (IsWayReference(r)) {
          const std::vector<OSMID>& w = ways->at(r.osm_id);
          for (Count i = 0; i + 1 < w.size(); ++i) {
            if (graph[w[i]].size() < 2 && graph[w[i + 1]].size() < 2) {
              graph[w[i]].push_back(w[i + 1]);
              graph[w[i + 1]].push_back(w[i]);
            }
          }
        }

      region.Clear();
      region.set_region_id(osm_id);
      region.set_options(GetOptions(kvs));

      for (const Reference& r : refs) {
        if (IsWayReference(r)) {
          const std::vector<OSMID>& w = ways->at(r.osm_id);
          for (Count i = 0; i < w.size(); ++i) {
            if (used.find(w[i]) == used.end()) {
              proto::Polygon* p = region.add_polygons();
              p->set_polygon_id(GeneratePolygonID());
              if (!strcmp(r.role, "inner"))
                p->set_type(proto::Polygon::TYPE_INNER);
              else
                p->set_type(proto::Polygon::TYPE_OUTER);
              SaveLocations(p, w[i]);
            }
          }
        }
      }

      if (region.polygons_size() > 0) {
        for (const KeyValue& x : kvs) {
          proto::KeyValue* kv = region.add_kvs();
          kv->set_key(x.key);
          kv->set_value(x.value);
        }

        buffer.clear();
        region.SerializeToString(&buffer);

        writer->Write(buffer);
      }
    }
  }
};

int main(int argc, char *argv[]) {
  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc < 2) {
    LogError("geo-base-convert") << "geo-base-convert <OSM.pbf>";
    return -1;
  }

  std::vector<std::string> args;
  geo_base::Options opts = geo_base::GetOpts(argc, argv, &args);

  std::unordered_set<OSMID> ways;

  {
    std::vector<WaysCallback> ways_callbacks(opts.jobs);
    MTProtobufParser<WaysCallback>(args[0], ways_callbacks)();

    for (WaysCallback& w : ways_callbacks) {
      ways.insert(w.ways.begin(), w.ways.end());
      w.ways.clear();
    }
  }

  LogInfo("geo-base-convert") << "Need ways count = " << ways.size();

  std::unordered_set<OSMID> node_ids;
  std::unordered_map<OSMID, std::vector<OSMID>> way_nodes;

  {
    std::vector<NodeIDsCallback> node_ids_callback;
    for (size_t i = 0; i < opts.jobs; ++i)
      node_ids_callback.emplace_back(ways);

    MTProtobufParser<NodeIDsCallback>(args[0], node_ids_callback)();

    for (NodeIDsCallback& n : node_ids_callback) {
      node_ids.insert(n.nodes.begin(), n.nodes.end());
      n.nodes.clear();
    }

    for (NodeIDsCallback& n : node_ids_callback) {
      way_nodes.insert(n.ways.begin(), n.ways.end());
      n.ways.clear();
    }
  }

  LogInfo("geo-base-convert") << "Need nodes count = " << node_ids.size();
  LogInfo("geo-base-convert") << "Way nodes count = " << way_nodes.size();

  std::unordered_map<OSMID, Location> nodes;

  {
    std::vector<NodesCallback> nodes_callback;
    for (size_t i = 0; i < opts.jobs; ++i)
      nodes_callback.emplace_back(node_ids);

    MTProtobufParser<NodesCallback>(args[0], nodes_callback)();

    for (NodesCallback& n : nodes_callback) {
      nodes.insert(n.nodes.begin(), n.nodes.end());
      n.nodes.clear();
    }
  }

  LogInfo("geo-base-convert") << "Nodes count = " << nodes.size();

  std::vector<Parser> parsers;
  ProtoWriter writer(STDOUT_FILENO);
  for (size_t i = 0; i < opts.jobs; ++i)
    parsers.emplace_back(nodes, way_nodes, writer, i, opts.jobs);

  MTProtobufParser<Parser>(args[0], parsers)();

  return 0;
}
