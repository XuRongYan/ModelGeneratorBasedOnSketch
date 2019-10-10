//
// Created by 徐溶延 on 2019-08-22.
//

#include "MeshIO.h"
/**
 * 读取mesh
 * @param mesh pmp提供的mesh
 * @param path mesh路径
 * @return
 */
int MeshIO::read_mesh(SurfaceMesh &mesh, const string &path) {
    bool success = mesh.read(path);
    if (!success) {
        logError() << "mesh读取失败，路径:" << path;
        exit(-1);
    }
    return 0;
}

/**
 * 保存mesh
 * @param mesh pmp提供的mesh
 * @param path mesh保存路径
 * @return
 */
int MeshIO::save_mesh(const SurfaceMesh &mesh, const string &path) {
    bool success = mesh.write(path);
    logInfo() << (success ? "保存成功" : "保存失败");
    return 0;
}
/**
 * 显示mesh
 * @param path mesh路径
 * @return
 */
int MeshIO::show_mesh(const string& path) {
    MeshViewer viewer(path.data(), 800, 450);
    viewer.load_mesh(path.data());
    return viewer.run();
}
