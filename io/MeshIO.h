//
// Created by 徐溶延 on 2019-08-22.
//

#ifndef SKETCHMODELTEST_MESHIO_H
#define SKETCHMODELTEST_MESHIO_H
#include <pmp/visualization/MeshViewer.h>
#include <pmp/SurfaceMeshIO.h>
#include <string>
#include <iostream>
#include "../utils/log_utils/Logger.h"

using namespace std;
using namespace pmp;

class MeshIO {
public:
    static int read_mesh(SurfaceMesh& mesh, const string& path);

    static int save_mesh(const SurfaceMesh& mesh, const string& path);

    static int show_mesh(const string& path);
};


#endif //SKETCHMODELTEST_MESHIO_H
