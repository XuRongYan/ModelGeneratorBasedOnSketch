//
// Created by 徐溶延 on 2019/9/18.
//

#ifndef MODELGENERATORBASEDONSKETCH_LUOPTIMIZER_H
#define MODELGENERATORBASEDONSKETCH_LUOPTIMIZER_H


#include "BaseOptimizer.h"

class LuOptimizer : public BaseOptimizer {
public:
    LuOptimizer(const MatrixXf &a, const MatrixXf &b) : BaseOptimizer(a, b) {}

    MatrixXf solve() override {
        return A.lu().solve(b);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_LUOPTIMIZER_H
