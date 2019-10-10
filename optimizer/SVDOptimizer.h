//
// Created by 徐溶延 on 2019/9/18.
//

#ifndef MODELGENERATORBASEDONSKETCH_SVDOPTIMIZER_H
#define MODELGENERATORBASEDONSKETCH_SVDOPTIMIZER_H


#include "BaseOptimizer.h"

class SVDOptimizer : public BaseOptimizer{
public:
    SVDOptimizer(const MatrixXf &a, const MatrixXf &b) : BaseOptimizer(a, b) {}

    MatrixXf solve() override {
        return A.bdcSvd().solve(b);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_SVDOPTIMIZER_H
