//
// Created by 徐溶延 on 2019/9/5.
//

#ifndef MODELGENERATORBASEDONSKETCH_LAPLACEWEIGHT_H
#define MODELGENERATORBASEDONSKETCH_LAPLACEWEIGHT_H

#include <pmp/SurfaceMesh.h>
#include <iostream>

using namespace std;
using namespace pmp;
class LaplaceWeight {
private:
    pmp::SurfaceMesh mesh;

protected:
    pmp::Point getPosition(Vertex v) {
        return mesh.position(v);
    }

public:
    explicit LaplaceWeight(const pmp::SurfaceMesh &mesh) : mesh(mesh) {}

    virtual ~LaplaceWeight() = default;

    virtual pmp::Scalar getWeight(Vertex v0, Vertex neighborV) = 0;

    const SurfaceMesh &getMesh() const {
        return mesh;
    }
};


#endif //MODELGENERATORBASEDONSKETCH_LAPLACEWEIGHT_H
