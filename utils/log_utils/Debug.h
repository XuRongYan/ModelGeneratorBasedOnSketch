//
// Created by 徐溶延 on 2019/8/23.
//

#ifndef SKETCHMODELTEST_DEBUG_H
#define SKETCHMODELTEST_DEBUG_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstdint>
#include <sstream>
#include <vector>
#include "LogContext.h"
#include "../Time.h"
#include <Eigen/Dense>
#include <Eigen/Sparse>

#define RESET "\033[0m"
#define BLACK "\033[30m" /* Black */
#define RED "\033[31m" /* Red */
#define GREEN "\033[32m" /* Green */
#define YELLOW "\033[33m" /* Yellow */
#define BLUE "\033[34m" /* Blue */
#define MAGENTA "\033[35m" /* Magenta */
#define CYAN "\033[36m" /* Cyan */
#define WHITE "\033[37m" /* White */
#define BOLDBLACK "\033[1m\033[30m" /* Bold Black */
#define BOLDRED "\033[1m\033[31m" /* Bold Red */
#define BOLDGREEN "\033[1m\033[32m" /* Bold Green */
#define BOLDYELLOW "\033[1m\033[33m" /* Bold Yellow */
#define BOLDBLUE "\033[1m\033[34m" /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define BOLDCYAN "\033[1m\033[36m" /* Bold Cyan */
#define BOLDWHITE "\033[1m\033[37m" /* Bold White */

using namespace std;
using namespace Eigen;

const bool isDebug = true;

enum LogType {
    Info,
    Warning,
    Error,
    Default
};

class Debug {
private:
    static Debug* m_instance;

public:
    //static ofstream ofs;
    struct Stream {
        ostringstream ss;
        bool space;
        LogContext context;
        LogType logType;

        Stream() : ss(), space(true), context() {}

        Stream(string *s) : ss(*s), space(true), context() {}
    } *stream;

    Debug() : stream(new Stream()) {}

    inline Debug(std::string *s) : stream(new Stream(s)) {}

    ~Debug();

    inline Debug &maybeSpace() {
        if (stream->space) {
            stream->ss << ' ';
        }
        return *this;
    }

    inline Debug &space() {
        stream->space = true;
        stream->ss << ' ';
        return *this;
    }

    inline Debug &no_space() {
        stream->space = false;
        return *this;
    }

    inline Debug &operator<<(bool t) {
        stream->ss << (t ? "true" : "false");
        return maybeSpace();
    }

    inline Debug &operator<<(char t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(int t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(float t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(double t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(unsigned long t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(long t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(signed short t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(unsigned short t) {
        stream->ss << t;
        return maybeSpace();
    }

    inline Debug &operator<<(const string &s) {
        stream->ss << s;
        return maybeSpace();
    }

    inline Debug &operator<<(const char *c) {
        stream->ss << c;
        return maybeSpace();
    }

    template<typename _Scalar, int _Rows, int _Cols, int _Options, int _MaxRows, int _MaxCols>
    inline Debug &operator<<(const Eigen::Matrix<_Scalar, _Rows, _Cols, _Options, _MaxRows, _MaxCols>& m) {
        stream->ss << m;
        return maybeSpace();
    }

    template<typename T>
    inline Debug &operator<<(const vector<T> &vec) {
        stream->ss << '(';
        for (int i = 0; i < vec.size(); ++i) {
            stream->ss << vec[i];
            stream->ss << ", ";
        }
        stream->ss << ')';
        return maybeSpace();
    }


    void log_to_console(LogType type, const LogContext& context, const string& logBuffer);
};


#endif //SKETCHMODELTEST_DEBUG_H
