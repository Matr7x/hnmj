#ifndef _XONE_LOGGING_PRINTER_H__
#define _XONE_LOGGING_PRINTER_H__

#ifdef __LOG4CPP__

#include <pthread.h>

#include <string>
#include <log4cpp/Category.hh>
#include <log4cpp/CategoryStream.hh>
#include <log4cpp/Priority.hh>
#include <log4cpp/threading/PThreads.hh>

#define LOGGING_TEMP_DEBUG(_fmt_, args...)          \
  { if (LogPrinter::isEnabledDebug){           \
    LogPrinter::Debug(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__,__FUNCTION__, __LINE__, LogPrinter::m_pid, pthread_self(), ##args); }}

#define LOGGING_DEBUG(_fmt_, args...)          \
  { if (LogPrinter::isEnabledDebug){           \
    LogPrinter::Debug(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__,__FUNCTION__, __LINE__, LogPrinter::m_pid, pthread_self(), ##args); }}

#define LOGGING_INFO(_fmt_, args...) \
  { if (LogPrinter::isEnabledInfo) { \
  LogPrinter::Info(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); } }

#define LOGGING_NOTICE(_fmt_, args...) \
  { if (LogPrinter::isEnabledNotice) { \
  LogPrinter::Notice(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); }}

#define LOGGING_WARN(_fmt_, args...) \
  { if (LogPrinter::isEnabledWarn) {\
  LogPrinter::Warn(" [%s:%s:%d][%u:%lu]: \t " _fmt_,  __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); }}

#define LOGGING_ERROR(_fmt_, args...) \
  { if (LogPrinter::isEnabledError) { \
  LogPrinter::Error(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); }}

#define LOGGING_CRIT(_fmt_, args...) \
  { if (LogPrinter::isEnabledCrit) { \
  LogPrinter::Crit(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); }}

#define LOGGING_FATAL(_fmt_, args...) \
  { if (LogPrinter::isEnabledFatal) {\
  LogPrinter::Fatal(" [%s:%s:%d][%u:%lu]: \t " _fmt_, __FILE__, __FUNCTION__, __LINE__, LogPrinter::m_pid,  pthread_self(), ##args); }}

#define LOGGING_PURE_INFO(_fmt_, args...) \
  { if (LogPrinter::isEnabledInfo) { \
  LogPrinter::Info(_fmt_, ##args); } }

class LogPrinter
{
public:
  static int Init(const char* configFile);

  static void Debug(const char* format, ...) __attribute__((format(printf, 1, 2)));
  static void Info(const char* format, ...)  __attribute__((format(printf, 1, 2)));
  static void Notice(const char* format, ...)__attribute__((format(printf, 1, 2)));
  static void Warn(const char* format, ...)  __attribute__((format(printf, 1, 2)));
  static void Error(const char* format, ...) __attribute__((format(printf, 1, 2)));
  static void Crit(const char* format, ...)  __attribute__((format(printf, 1, 2)));
  static void Fatal(const char* format, ...) __attribute__((format(printf, 1, 2)));

  static void SetProgName(const std::string& name);
  static void SetModName(const std::string& name);
  static void Shutdown();

  static int isEnabledDebug;  
  static int isEnabledInfo;  
  static int isEnabledNotice;  
  static int isEnabledWarn;  
  static int isEnabledError;  
  static int isEnabledCrit;  
  static int isEnabledFatal;  
  
  static uint32_t m_pid;  

  static log4cpp::Category* m_root;
  static log4cpp::threading::Mutex* m_mutex;
  static std::string* m_progName;
  static std::string* m_modName;
  static std::string FormatMessage(const std::string& msg);
};    

#else 

#include <errno.h>
#include <pthread.h>
#include <cstdio>
#include <cstring>

#define XONE_LOGGING(_level_, _io_, _fmt_, args...)                               \
    do {                                                                          \
     fprintf(_io_, _level_"[%s:%s:%d][%lu]"_fmt_"\n",                             \
                 __FILE__, __FUNCTION__, __LINE__, pthread_self(), ##args);       \
    } while (0);

#define LOGGING_PURE_INFO(_fmt_, args...)                                         \
    do {                                                                          \
     fprintf(stderr, "[INFO]"_fmt_"\n", ##args);                                  \
    } while (0);

#ifdef __ODPS_DEBUG__
#define LOGGING_DEBUG(_fmt_, args...)                                             \
  XONE_LOGGING("[DEBUG]", stdout, _fmt_, ##args)
#else
#define LOGGING_DEBUG(_fmt_, args...)
#endif

#define LOGGING_TEMP_DEBUG(_fmt_, args...)                                        \
  XONE_LOGGING("[TEMP_DEBUG]", stderr, _fmt_, ##args) 

#define LOGGING_INFO(_fmt_, args...)                                              \
  XONE_LOGGING("[INFO]", stdout, _fmt_, ##args) 

#define LOGGING_NOTICE(_fmt_, args...)                                            \
  XONE_LOGGING("[NOTICE]", stdout, _fmt_, ##args) 

#define LOGGING_VISUAL(_fmt_, args...)                                            \
  XONE_LOGGING("[VISUAL]", stderr, _fmt_, ##args) 

#define LOGGING_WARN(_fmt_, args...)                                              \
  XONE_LOGGING("[WARN]", stderr, _fmt_, ##args) 

#define LOGGING_ERROR(_fmt_, args...)                                             \
  XONE_LOGGING("[ERROR]", stderr, _fmt_, ##args) 

#define LOGGING_CRIT(_fmt_, args...)                                              \
  XONE_LOGGING("[CRIT]", stderr, _fmt_, ##args) 

#define LOGGING_FATAL(_fmt_, args...)                                             \
  XONE_LOGGING("[FATAL]", stderr, _fmt_, ##args) 

#endif  // __LOG4CPP__

#endif  // _XONE_LOGGING_PRINTER_H__
