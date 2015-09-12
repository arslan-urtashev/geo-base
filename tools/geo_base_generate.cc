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

#include <atomic>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <thread>

#include "geo_base_generate.h"
#include "io.h"
#include "log.h"

using namespace geo_base;

typedef std::vector<Location> Locations;
typedef std::vector<std::string> Blobs;

struct Reader {
  GeoBaseGenerate *generate;
  std::atomic<bool> readed;

  Reader(GeoBaseGenerate* g) :
      generate(g),
      readed(false) {
  }

  void operator () () {
    ProtoParser(STDIN_FILENO)([&] (const proto::Region& region) {
      generate->Update(region);
    });

    readed = true;
  }

  bool Readed() const {
    return readed;
  }
};

int main(int argc, char *argv[]) {
  std::cerr << std::fixed << std::setprecision(2);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc != 2) {
    LogError("geo-base-generate") << "geo-base-generate <geodata.dat>";
    return -1;
  }

  try {
    StopWatch stop_watch;
    stop_watch.CheckPoint();

    GeoBaseGenerate generate(argv[1]);

    LogInfo("geo-base-generate") << "Run generate...";

    Reader reader(&generate);
    std::thread thread(std::ref(reader));

    while (!reader.Readed()) {
      const GenerateContext& ctx = generate.Context();
      LogStatus("geo-base-generate", "status") << "Polygons = " <<
          ctx.polygons.size() << ", " << ctx.MemoryUsage() << " MB";
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    thread.join();
    LogStatus::Clear();

    LogInfo("geo-base-generate") << "Save geodata...";

    generate.Save();
    generate.ShowBase(std::cerr);

    LogInfo("geo-base-generate") << "Done, spent = "
        << stop_watch.CheckPoint() * 1.0 / 60. << " MIN";

  } catch (const std::exception& e) {
    LogError("geo-base-generate", "EXCEPTION") << e.what();
  }

  return 0;
}
