//
// Created by 徐溶延 on 2019-08-22.
//
#include "Time.h"

//TODO 实现不优雅
void Time::start() {
    if (isStart) {
        //logWarning() << "计时已经开始，请使用Time::end()";
        return;
    }
    start_time = clock();
    isStart = true;
}

void Time::end() {
    if (!isStart) {
        //logWarning() << "计时还未开始，请使用Time::start()";
        return;
    }
    end_time = clock();
    //logInfo() << "算法用时:" << (end_time - start_time) << "ms";
    isStart = false;
}
