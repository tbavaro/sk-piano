#ifndef __INCLUDED_LOG_H
#define __INCLUDED_LOG_H

#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>

#define ENABLE_LOG_DEBUG
#define ENABLE_LOG_INFO

#define __LOG(f, level, fmt, args...) \
  { \
    fprintf(f, "[%s] ", level); \
    fprintf(f, fmt, args); \
    fprintf(f, "\n"); \
  }

#ifdef ENABLE_LOG_DEBUG
#define logDebug(fmt, args...) __LOG(stdout, "DEBUG", fmt, args)
#else
#define logDebug(fmt, args...)
#endif

#ifdef ENABLE_LOG_DEBUG
#define logInfo(fmt, args...) __LOG(stdout, "INFO", fmt, args)
#else
#define logInfo(fmt, args...)
#endif

#define logError(fmt, args...) __LOG(stderr, "ERROR", fmt, args)

#define RUN_LOGGING_EXCEPTION(block) \
  { \
    try { \
      block \
    } catch (exception& e) { \
      __LOG(stderr, "EXCEPTION", "%s", e.what()); \
      throw e; \
    } \
  }

#endif
