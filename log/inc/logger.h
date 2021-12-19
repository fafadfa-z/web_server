#ifndef LOGGER_H
#define LOGGER_H

#include "LogStream.h"
#include "LogThread.h"
#include <functional>



namespace log
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
    void dealLogStr(const char *, std::string &buf, int maxSize = 15);

    std::string fileN;  //文件名
    std::string funN;   //函数名
    std::string levelS; // log等级

    LogStream stream_;

    static const int maxNameSize = 25; //文件名等缓冲区的最大容量
  };

       LogStream& end(LogStream& stream);

}  // namespace log

const bool enableDebug = 1;

const bool enableHttpDebug = 1;

const log::LogLevel logLevel = log::LogLevel::INFO_;

#define LOG_INFO                        \
  if (logLevel <= log::LogLevel::INFO_) \
  log::entity->logger->receive(__FILE__, __LINE__, __func__, log::INFO_)

#define LOG_DEBUG                        \
  if (logLevel <= log::LogLevel::DEBUG_) \
  log::entity->logger->receive(__FILE__, __LINE__, __func__, log::DEBUG_)

#define LOG_FATAL                        \
  if (logLevel <= log::LogLevel::FATAL_) \
  log::entity->logger->receive(__FILE__, __LINE__, __func__, log::FATAL_)

#define LOG_HTTP       \
  if (enableHttpDebug) \
  log::entity->logger->receive(__FILE__, __LINE__, __func__, log::HTTP_)

#define LOG_COUT std::cout


#endif
