//
// Created by 徐溶延 on 2019-08-22.
//

#ifndef SKETCHMODELTEST_TIME_H

#define SKETCHMODELTEST_TIME_H
#include <ctime>
#include <string>
#include <iostream>


using namespace std;


class Time {
    typedef long clock_t;
private:
    bool isStart = false;
    clock_t start_time;
    clock_t end_time;

public:
    static string get_time(const char *format = "%Y/%m/&d %H:%M:%S") {
        time_t timep;
        time(&timep);
        char tmp[64];
        strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
        return string(tmp);
    }
    void start();
    void end();

    clock_t getStartTime() const {
        return start_time;
    }

    void setStartTime(clock_t startTime) {
        start_time = startTime;
    }

    clock_t getEndTime() const {
        return end_time;
    }

    void setEndTime(clock_t endTime) {
        end_time = endTime;
    }

};




#endif //SKETCHMODELTEST_TIME_H
