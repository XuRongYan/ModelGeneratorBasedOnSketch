//
// Created by 徐溶延 on 2019/9/8.
//
#include <gtest/gtest.h>
#include "../optimizer/LLTOptimizer.h"
#include "../optimizer/LdltOptimizer.h"

class OptimizerTest : public ::testing::Test {
public:

    MatrixXf A, b, x;
    void generate_A_b() {
        A.resize(2, 2);
        b.resize(2, 1);
        x.resize(2, 1);
        A << 1, -1,
             -1, 5;
        b << 3,
             -7;
        x << 2,
             -1;
    }
};

TEST_F(OptimizerTest, LDLTSolveTest) {
    generate_A_b();
    LdltOptimizer ldltOptimizer = LdltOptimizer(A, b);
    EXPECT_EQ(ldltOptimizer.solve(), x);
}

TEST_F(OptimizerTest, LLTSolveTest) {
    generate_A_b();
    LLTOptimizer optimizer = LLTOptimizer(A, b);
    EXPECT_EQ(optimizer.solve(), x);
}
