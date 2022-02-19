#ifndef _LOGGER_H
#define _LOGGER_H

#include "log_stream.h"
#include "log_thread.h"

#include <functional>
#include <cassert>
#include <iostream>



namespace Log
{
  extern LogThread *entity;
 
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

    const char *levelToStr(const LogLevel);

    LogStream &receive(const char *fileName, int line, const char *funName, LogLevel level);

    unsigned int findFileName(const char *);

    std::string dealFunName(const char *funName);

  private:
    void dealLogStr(const char *, std::string &buf, unsigned long maxSize = 15);

    std::string fileN;  //文件名
    std::string funN;   //函数名
    std::string levelS; // log等级

    LogStream stream_;

    static const int maxNameSize = 25; //文件名等缓冲区的最大容量
  };

  LogStream& end(LogStream& stream);

}  // namespace Log

const bool enableDebug = 1;

const bool enableHttpDebug = 1;

const Log::LogLevel logLevel = Log::LogLevel::DEBUG_;

#define LOG_INFO                        \
  if (logLevel <= Log::LogLevel::INFO_) \
  Log::entity->logger->receive(__FILE__, __LINE__, __func__, Log::INFO_)

#define LOG_DEBUG                        \
  if (logLevel <= Log::LogLevel::DEBUG_) \
  Log::entity->logger->receive(__FILE__, __LINE__, __func__, Log::DEBUG_)

#define LOG_FATAL                        \
  if (logLevel <= Log::LogLevel::FATAL_) \
  Log::entity->logger->receive(__FILE__, __LINE__, __func__, Log::FATAL_)

#define LOG_HTTP       \
  if (enableHttpDebug) \
  Log::entity->logger->receive(__FILE__, __LINE__, __func__, Log::HTTP_)

#define LOG_COUT std::cout


#endif
