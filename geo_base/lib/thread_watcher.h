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

#pragma once

#include <thread>
#include <atomic>

#include <geo_base/lib/common.h>
#include <geo_base/lib/log.h>

namespace geo_base {

namespace {

enum operation_t {
    OPERATION_CLEAR,
    OPERATION_GO_UP,
};

char const *operations[] = {
    "\033[K",  // OPERATION_CLEAR
    "\033[1A", // OPERATION_GO_UP
};

} // namespace

template<typename container_t, typename callback_t>
class thread_watcher_t {
public:
    explicit thread_watcher_t(container_t const &container)
        : container_(&container)
        , callback_()
        , thread_()
        , running_(false)
    { }

    void run()
    {
        thread_watcher_t *watcher = this;
        thread_ = std::thread([watcher] () {
            watcher->job();
        });
        running_ = true;
    }

    void stop()
    {
        running_ = false;
        thread_.join();
    }

private:
    void job()
    {
        while (running_) {
            int const fd = log_fd();

            for (size_t i = 0; i < container_->size(); ++i)
                dprintf(fd, "(%.2lu)=> %s\n", i + 1, callback_(container_->at(i)).c_str());

            std::this_thread::sleep_for(std::chrono::milliseconds(100));

            for (size_t i = 0; i < container_->size(); ++i)
                dprintf(fd, "%s%s", operations[OPERATION_CLEAR], operations[OPERATION_GO_UP]);
        }
    }

    container_t const *container_;
    callback_t callback_;

    std::thread thread_;
    std::atomic<bool> running_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(thread_watcher_t);
};

}
