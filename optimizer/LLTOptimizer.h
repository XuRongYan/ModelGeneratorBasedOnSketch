//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_LLTOPTIMIZER_H
#define MODELGENERATORBASEDONSKETCH_LLTOPTIMIZER_H


#include "BaseOptimizer.h"

class LLTOptimizer : public BaseOptimizer{
public:
    LLTOptimizer(const MatrixXf &a, const MatrixXf &b) : BaseOptimizer(a, b) {}

    MatrixXf solve() override {
        return A.llt().solve(b);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_LLTOPTIMIZER_H
