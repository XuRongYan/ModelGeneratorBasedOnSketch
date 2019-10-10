//
// Created by 徐溶延 on 2019/9/10.
//

#ifndef MODELGENERATORBASEDONSKETCH_MESHSPHERICALCOORDINATE_H
#define MODELGENERATORBASEDONSKETCH_MESHSPHERICALCOORDINATE_H

#include <pmp/SurfaceMesh.h>
#include "SphericalCoordinate.h"

using namespace pmp;

class MeshSphericalCoordinate : public SphericalCoordinate {
private:
    SurfaceMesh mesh;
public:
    explicit MeshSphericalCoordinate(const SurfaceMesh &mesh) : mesh(mesh) {
        initPhis();
        initThetas();
    }

private:
    void initPhis() {
        for(auto v : mesh.vertices()) {
            pmp::Point p = mesh.position(v);
            pmp::Scalar x = p[0];
            pmp::Scalar y = p[1];
            phis.push_back(atan2(y, x));
        }
    }

    void initThetas() {
        for(auto v : mesh.vertices()) {
            pmp::Point p = mesh.position(v);
            pmp::Scalar x = p[0];
            pmp::Scalar y = p[1];
            pmp::Scalar z = p[2];
            pmp::Scalar r = sqrt(x * x + y * y + z * z);
            thetas.push_back(acos(z / r));
        }
    }
};


#endif //MODELGENERATORBASEDONSKETCH_MESHSPHERICALCOORDINATE_H
