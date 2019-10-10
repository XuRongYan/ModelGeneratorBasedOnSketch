//
// Created by 徐溶延 on 2019/8/23.
//

#include "Debug.h"

void Debug::log_to_console(LogType type, const LogContext &context, const string &logBuffer) {
    string logString;
    string nowTime = "[";
    nowTime.append(Time::get_time());
    nowTime.append("]: ");
    logString.append(nowTime);
    logString.append("[");
    if (context.file != nullptr) {
        logString.append(context.file);
        logString.append(" ");
    }
    if (context.function != nullptr) {
        logString.append(context.function);
        logString.append("() ");
    }
    if (context.line != 0) {
        logString.append("line:");
        logString.append(to_string(context.line));
        //logString.append(" ");
    }
    logString.append("] ");
    switch (type) {
        case Error:
            logString.append("Error/");
            break;
        case Info:
            logString.append("Info/");
            break;
        case Warning:
            logString.append("Warning/");
            break;
        case LogType::Default:
            logString.append("Debug/");
            break;
        default:
            break;
    }
    logString.append(logBuffer);
    switch (type) {
        case Error:
            //if (ofs) ofs << logString << endl;
            cout << RED << logString << RESET << endl;
            break;
        case Info:
            //if (ofs) ofs << logString << endl;
            cout << GREEN << logString << RESET << endl;
            break;
        case Warning:
            //if (ofs) ofs << logString << endl;
            cout << YELLOW << logString << RESET << endl;
            break;
        case LogType::Default:
            if (isDebug) {
                //if (ofs) ofs << logString << endl;
                cout << WHITE << logString << RESET << endl;
            }
            break;
        default:
            break;
    }
}

Debug::~Debug() {
    log_to_console(stream->logType, stream->context, stream->ss.str());
    delete stream;
}


