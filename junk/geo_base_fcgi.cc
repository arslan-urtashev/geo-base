// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <fcgiapp.h>
#include <errno.h>
#include <geo_base/proto_util/proto_reader.h>

using namespace geo_base;

static char const *DAEMON_ADDRESS = "localhost:3388";
static int const DAEMON_BACKLOG = 16;

template<typename input_t>
static void get_request(std::string *s, input_t input)
{
    static size_t const N_BYTES = 64;

    while (true) {
        char buffer[N_BYTES];

        size_t const bytes = FCGX_GetStr(buffer, N_BYTES, input);
        if (!bytes)
            break;

        s->append(buffer, bytes);
    }
}

int main(int argc, char *argv[])
{
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc != 2) {
        log_error("USAGE: geo-base-fcgi <geo-base.pbf>");
        return -1;
    }

    proto_reader_t reader(argv[1]);
    reader.generate_index();

    if (FCGX_Init()) {
        log_error("Unable init fcgx");
        return -1;
    }

    log_info("Fcgx is successfully initialized");

    int socket = FCGX_OpenSocket(DAEMON_ADDRESS, DAEMON_BACKLOG);
    if (socket < 0) {
        log_error("Unable open fcgx socket");
        return -1;
    }

    log_info("Socket %d is successfully opened", socket);

    FCGX_Request request;
    if (FCGX_InitRequest(&request, socket, 0)) {
        log_error("Unable init fcgx request");
        return -1;
    }

    log_info("Fcgx request is successfully initialized");
    log_info("Ready to accept requests on %s", DAEMON_ADDRESS);

    std::string req;
    std::string reply;

    while (true) {
        log_info("Accepting request...");

        int const error = FCGX_Accept_r(&request);
        if (error != 0) {
            log_error("Unable accept request: %s (%d)", strerror(errno), error);
            break;
        }

        log_info("Request accepted");

        req.clear();
        get_request(&req, request.in);

        log_info("Request: %s", req.data());

        geo_id_t geo_id = strtoull(req.data(), nullptr, 10);

        reply.clear();
        reply += "{";

        reader.region(geo_id, [&] (proto::region_t const &region) {
            reply += "\"polygons\":[";

            bool is_first_polygon = true;
            for (proto::polygon_t const &p : region.polygons()) {
                if (!is_first_polygon)
                    reply += ",";
                reply += "[";

                bool is_first_location = true;
                for (proto::location_t const &l : p.locations()) {
                    if (!is_first_location)
                        reply += ",";

                    reply += "[" + std::to_string(l.lat()) + "," + std::to_string(l.lon()) + "]";

                    is_first_location = false;
                }

                reply += "]";
                is_first_polygon = false;
            }

            reply += "]";
        });

        reply += "}";

        FCGX_PutS(("Content-Length: " + std::to_string(reply.size()) + "\r\n").data(), request.out);
        FCGX_PutS("Content-type: application/json\r\n", request.out);
        FCGX_PutS("\r\n", request.out);
        FCGX_PutS(reply.data(), request.out);

        FCGX_Finish_r(&request);

        log_info("Reply printed");
    }

    return 0;
}
