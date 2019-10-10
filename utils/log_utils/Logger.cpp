//
// Created by 徐溶延 on 2019/8/23.
//

#include "Logger.h"

Debug Logger::warning() const {
    Debug dbg = Debug();
    LogContext &ctxt = dbg.stream->context;
    ctxt.copy(context);
    dbg.stream->logType = Warning;
    return dbg;
}

Debug Logger::info() const {
    Debug dbg = Debug();
    LogContext &ctxt = dbg.stream->context;
    ctxt.copy(context);
    dbg.stream->logType = Info;
    return dbg;

}

Debug Logger::debug() const {
    Debug dbg = Debug();
    LogContext &ctxt = dbg.stream->context;
    ctxt.copy(context);
    dbg.stream->logType = LogType::Default;
    return dbg;
}

Debug Logger::error() const {
    Debug dbg = Debug();
    LogContext &ctxt = dbg.stream->context;
    ctxt.copy(context);
    dbg.stream->logType = Error;
    return dbg;
}
