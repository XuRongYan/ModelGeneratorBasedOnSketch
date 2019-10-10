//
// Created by 徐溶延 on 2019/9/8.
//

#ifndef MODELGENERATORBASEDONSKETCH_LDLTOPTIMIZER_H
#define MODELGENERATORBASEDONSKETCH_LDLTOPTIMIZER_H

#include <Eigen/Core>

#include "BaseOptimizer.h"
using namespace Eigen;
class LdltOptimizer : public BaseOptimizer{
public:
    LdltOptimizer(const MatrixXf &a, const MatrixXf &b) : BaseOptimizer(a, b) {}

    MatrixXf solve() override {
        return A.ldlt().solve(b);
    }
};


#endif //MODELGENERATORBASEDONSKETCH_LDLTOPTIMIZER_H
