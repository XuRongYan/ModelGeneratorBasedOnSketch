#include <iostream>
#include <gtest/gtest.h>
#include "ModelFactory.h"
using namespace std;

int main(int argc, char **argv) {
    //::testing::InitGoogleTest(&argc, argv);
    //return RUN_ALL_TESTS();
    ModelFactory factory = ModelFactory::Builder()
                                        .setImgPath("animx.tif")
                                        .setResultPath("result.obj")
                                        .setLambdaXy(0.0005)
                                        .setLambdaZ(1)
                                        .setLevel(15)
                                        .setInterval(1)
                                        .build();
    factory.buildModel();
    MeshIO::show_mesh(factory.getResultPath());
    return 0;
}