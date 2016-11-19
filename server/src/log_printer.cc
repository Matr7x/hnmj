#ifdef __LOG4CPP__

#include <log4cpp/PropertyConfigurator.hh>
#include <iostream>
#ifndef WIN32
#include <unistd.h>
#else
#include <process.h>
#endif
#include <stdio.h>

#include "log_printer.h"
#include "log4cpp/OstreamAppender.hh"


log4cpp::Category* LogPrinter::m_root = NULL;
std::string* LogPrinter::m_progName = NULL;
std::string* LogPrinter::m_modName = NULL;
log4cpp::threading::Mutex* LogPrinter::m_mutex = NULL;

int LogPrinter::isEnabledDebug = 0;  
int LogPrinter::isEnabledInfo = 0;  
int LogPrinter::isEnabledNotice = 0;  
int LogPrinter::isEnabledWarn = 0;  
int LogPrinter::isEnabledError = 0;  
int LogPrinter::isEnabledCrit = 0;  
int LogPrinter::isEnabledFatal = 0;  

uint32_t LogPrinter::m_pid = 0;

void LogPrinter::SetProgName(const std::string& name) {
  *(LogPrinter::m_progName)=name;
}

void LogPrinter::SetModName(const std::string& name) {
  *(LogPrinter::m_modName)=name;
}

int LogPrinter::Init(const char* configFile) {
  try {
    std::string confFileName = configFile;
    log4cpp::PropertyConfigurator::configure(confFileName);
  } catch (log4cpp::ConfigureFailure& f) {
    std::cerr << "Log4Cpp Configure exception happened. [error=" 
              << f.what() << "]" << std::endl;;
    return -1;
  }

  if(LogPrinter::m_mutex == NULL) {
    LogPrinter::m_mutex = new log4cpp::threading::Mutex();
  }
  if(LogPrinter::m_progName == NULL) {
    LogPrinter::m_progName = new std::string;
  }
  if(LogPrinter::m_modName == NULL) {
    LogPrinter::m_modName = new std::string;
  }

  LogPrinter::m_root=&(log4cpp::Category::getRoot());

  LogPrinter::isEnabledDebug = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::DEBUG)) {
    LogPrinter::isEnabledDebug = 1;
  } 

  LogPrinter::isEnabledInfo = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::INFO)) {
    LogPrinter::isEnabledInfo = 1;
  } 

  LogPrinter::isEnabledNotice = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::NOTICE)) {
    LogPrinter::isEnabledNotice = 1;
  } 

  LogPrinter::isEnabledWarn = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::WARN)) {
    LogPrinter::isEnabledWarn = 1;
  } 

  LogPrinter::isEnabledError = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::ERROR)) {
    LogPrinter::isEnabledError = 1;
  } 

  LogPrinter::isEnabledCrit = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::CRIT)) {
    LogPrinter::isEnabledCrit = 1;
  } 

  LogPrinter::isEnabledFatal = 0;  
  if (LogPrinter::m_root->isPriorityEnabled(log4cpp::Priority::FATAL)) {
    LogPrinter::isEnabledFatal = 1;
  } 
  LogPrinter::m_pid = getpid();

  return 0;
}

void LogPrinter::Shutdown() {
  log4cpp::Category::shutdown(); 
  if(m_mutex != NULL) {
    delete m_mutex; 
  }
  if(m_progName != NULL) {
    delete m_progName;
  }
  if(m_modName != NULL) {
    delete m_modName;
  }
}

void LogPrinter::Warn(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap, format);
    LogPrinter::m_root->logva(log4cpp::Priority::WARN, (format), ap);
    va_end(ap);
  }
}

void LogPrinter::Error(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap, format);
    LogPrinter::m_root->logva(log4cpp::Priority::ERROR, (format), ap);
    va_end(ap);
  }
}

void LogPrinter::Info(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap,format);
    LogPrinter::m_root->logva(log4cpp::Priority::INFO,(format),ap);
    va_end(ap);
  }
}

void LogPrinter::Notice(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap,format);
    LogPrinter::m_root->logva(log4cpp::Priority::NOTICE,(format),ap);
    va_end(ap);
  }
}
void LogPrinter::Debug(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap,format);
    LogPrinter::m_root->logva(log4cpp::Priority::DEBUG,(format),ap);
    va_end(ap);
  }
}
void LogPrinter::Crit(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap,format);
    LogPrinter::m_root->logva(log4cpp::Priority::CRIT,(format),ap);
    va_end(ap);
  }
}

void LogPrinter::Fatal(const char* format, ...)
{
  {
    log4cpp::threading::ScopedLock lock(*m_mutex);
    va_list ap;
    va_start(ap,format);
    LogPrinter::m_root->logva(log4cpp::Priority::FATAL,(format),ap);
    va_end(ap);
  }
}

#endif
