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
//
// Simple thread safe logger.
// Usage:
//   LogError("geo-base-generate", "hello") << "Hello, world!";
// WARNING: Bad works with recursive lock!

#ifndef GEO_BASE_LOG_H_
#define GEO_BASE_LOG_H_

#include "io.h"
#include "watch.h"

#include <mutex>

namespace geo_base {

class Log {
 public:
  enum Level {
    LEVEL_NULL = -1,
    LEVEL_ERROR,
    LEVEL_WARNING,
    LEVEL_INFO,
    LEVEL_DEBUG
  };

  enum Color { 
    COLOR_ENABLE,
    COLOR_DISABLE
  };

  Level level() const {
    return level_;
  }

  Color color() const {
    return color_;
  }

  OutputStream& out() {
    return *out_;
  }

  void Lock() {
    lock_.lock();
  }

  void Unlock() {
    lock_.unlock();
  }

  static Log& GetInst();

 private:
  Log() :
      color_(COLOR_ENABLE),
      level_(LEVEL_NULL),
      out_(NULL) {
  }

  Color color_;
  Level level_;
  std::recursive_mutex lock_;
  OutputStream* out_;

  friend void LogInit(OutputStream& out, Log::Level level, Log::Color color);
};

void LogInit(OutputStream& out, Log::Level level, Log::Color color);

class LogOutput {
 public:
  template<typename T>
  LogOutput &operator << (const T& x) {
    if (static_cast<int>(level) <= static_cast<int>(Log::GetInst().level()))
      Log::GetInst().out() << x;
    return *this;
  }

 protected:
  LogOutput(char begin, char end, Log::Level level) :
      begin(begin),
      end(end),
      level(level) {
    if (static_cast<int>(level) <= static_cast<int>(Log::GetInst().level())) {
      Log::GetInst().Lock();
      (*this) << begin;
    }
  }

  ~LogOutput() {
    if (static_cast<int>(level) <= static_cast<int>(Log::GetInst().level())) {
      (*this) << end;
      Log::GetInst().out().flush();
      Log::GetInst().Unlock();
    }
  }

  char begin;
  char end;
  Log::Level level;
};

class LogColor : public LogOutput {
 protected:
  LogColor(int color1, char begin, char end, Log::Level level) :
      LogOutput(begin, end, level),
      color(color1) {
    if (Log::GetInst().color() == Log::COLOR_ENABLE)
      (*this) << "\033[" << color << "m";
  }

  ~LogColor() {
    if (Log::GetInst().color() == Log::COLOR_ENABLE)
      (*this) << "\033[0m";
  }

 private:
  int color;
};

class LogLevel : public LogColor {
 protected:
  LogLevel(int color, char begin, char end, Log::Level level) :
      LogColor(color, begin, end, level) {
    static const char *levels[] = {
      "[  error]",
      "[warning]",
      "[   info]",
      "[  debug]"
    };

    if (level != Log::LEVEL_NULL) {
      uint8_t l = static_cast<uint8_t>(level);
      if (l < sizeof(levels))
        (*this) << levels[l] << " ";
    }
  }
};

class LogTime : public LogLevel {
 protected:
  LogTime(int color, char begin, char end, Log::Level level) :
      LogLevel(color, begin, end, level) {
    (*this) << Watch();
  }
};

class LogContext : public LogTime {
 protected:
  template<typename... Args>
  LogContext(int color, char begin, char end, Log::Level level, Args... args) :
      LogTime(color, begin, end, level) {
    WriteContext(args...);
  }

 private:
  void WriteContext() {
  }

  template<typename T, typename... Args>
  void WriteContext(const T& x, Args... args) {
    (*this) << "[" << x << "] ";
    WriteContext(args...);
  }
};

#define LOG_DEF(Type, color, level) \
  class Log##Type : public LogContext { \
   public: \
    template<typename... Args> \
    Log##Type(Args... args) : \
        LogContext(color, '\r', '\n', level, args...) { \
    } \
  };

LOG_DEF(Error, 31, Log::LEVEL_ERROR);
LOG_DEF(Info, 32, Log::LEVEL_INFO);
LOG_DEF(Warning, 33, Log::LEVEL_WARNING);
LOG_DEF(Debug, 37, Log::LEVEL_DEBUG);

#undef LOG_DEF

class LogStatus : public LogContext {
 public:
  template<typename... Args>
  LogStatus(Args... args) :
      LogContext(32, '\r', ' ', Log::LEVEL_INFO, args...) {
  }

  static void Clear() {
    Log::GetInst().out() << '\n';
    Log::GetInst().out().flush();
  }
};

} // namespace geo_base

#endif // GEO_BASE_LOG_H_
