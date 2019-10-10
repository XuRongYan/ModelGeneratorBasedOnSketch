//
// Created by 徐溶延 on 2019/8/23.
//

#ifndef SKETCHMODELTEST_LOGCONTEXT_H
#define SKETCHMODELTEST_LOGCONTEXT_H

#include <string>
using namespace std;
class LogContext {
public:
    int line;
    const char *file;
    const char *function;
public:
    LogContext() : line(0), file(nullptr), function(nullptr) {}

    LogContext(const char *fileName, const char *funcName, int lineNum) : file(fileName), function(funcName),
                                                                          line(lineNum) {}
    void copy(const LogContext& logContext) {
        this->file = logContext.file;
        this->line = logContext.line;
        this->function = logContext.function;
    }

private:
    friend class Logger;
    friend class Debug;
};


#endif //SKETCHMODELTEST_LOGCONTEXT_H
