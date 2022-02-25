#ifndef _LOGGER_H
#define _LOGGER_H

#include "log_stream.h"
#include "log_thread.h"

#include <functional>
#include <cassert>
#include <iostream>



namespace Log
{
  extern  LogThread *entity;
 
  LogThread* init();

  enum LogLevel
  {
    INFO_,
    DEBUG_,
    FATAL_,
    HTTP_
  };

  class Logger
  {
  public:
    Logger(std::function<void(char *, int)>);

    constexpr char const *levelToStr();

    LogStream &receive(const char *fileName, int line, const char *funName);

    unsigned int findFileName(const char *);

    std::string dealFunName(const char *funName);

    static void resizeBuf()
    {
      fileN.resize(maxNameSize);
      funN.resize(maxNameSize);
    }

  private:
    void dealLogStr(const char *, std::string &buf,int maxSize = 15);

    inline static thread_local std::string fileN;  //文件名
    inline static thread_local std::string funN;   //函数名

    LogStream stream_;

    static const int maxNameSize = 25; //文件名等缓冲区的最大容量
  };

  LogStream& end(LogStream& stream);  //流结束标志

}  // namespace Log

const bool enableHttpDebug = false;

constexpr Log::LogLevel logLevel = Log::LogLevel::DEBUG_;

#define LOG_INFO                        \
  if constexpr(logLevel <= Log::LogLevel::INFO_) \
  Log::entity->logger_->receive(__FILE__, __LINE__, __func__)

#define LOG_DEBUG                        \
  if constexpr(logLevel <= Log::LogLevel::DEBUG_) \
  Log::entity->logger_->receive(__FILE__, __LINE__, __func__)

#define LOG_FATAL                        \
  if constexpr(logLevel <= Log::LogLevel::FATAL_) \
  Log::entity->logger_->receive(__FILE__, __LINE__, __func__)

#define LOG_HTTP       \
  if constexpr(enableHttpDebug) \
  Log::entity->logger_ ->receive(__FILE__, __LINE__, __func__)

#define LOG_COUT std::cout


#endif
