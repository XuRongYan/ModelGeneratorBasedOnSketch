//
// Created by 徐溶延 on 2019/8/23.
//

#ifndef SKETCHMODELTEST_LOGGER_H
#define SKETCHMODELTEST_LOGGER_H
#define logDebug Logger(__FILE__, __FUNCTION__, __LINE__).debug
#define logInfo Logger(__FILE__, __FUNCTION__, __LINE__).info
#define logWarning Logger(__FILE__, __FUNCTION__, __LINE__).warning
#define logError Logger(__FILE__, __FUNCTION__, __LINE__).error
#include "Debug.h"
#include "LogContext.h"
#include "../Time.h"
class Logger {
private:
    LogContext context;
public:
    static ofstream ofs;
    Logger() : context() {}

    Logger(const char *fileName, const char *funcName, int lineNum) : context(fileName, funcName, lineNum) {}

    Debug debug() const;

    Debug info() const;

    Debug warning() const;

    Debug error() const;
};


#endif //SKETCHMODELTEST_LOGGER_H
