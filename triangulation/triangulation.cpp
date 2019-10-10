#include "triangulation.h"

#include <sstream>
#include <algorithm>

#ifdef ANSI_DECLARATORS
 #  define MY_PREVIOUSLY_DEFINED_ANSI_DECLARATORS ANSI_DECLARATORS
#  undef ANSI_DECLARATORS
#endif
#ifdef REAL
#  define MY_PREVIOUSLY_DEFINED_REAL REAL
#  undef REAL
#endif
#ifdef VOID
#  define MY_PREVIOUSLY_DEFINED_VOID VOID
#  undef VOID
#endif
#define ANSI_DECLARATORS
#define REAL double
#define VOID int

extern "C"
{
#include "triangle.h"
}

#undef ANSI_DECLARATORS
#ifdef MY_PREVIOUSLY_DEFINED_ANSI_DECLARATORS
#  define ANSI_DECLARATORS MY_PREVIOUSLY_DEFINED_ANSI_DECLARATORS
#endif

#undef REAL
#ifdef MY_PREVIOUSLY_DEFINED_REAL
#  define REAL MY_PREVIOUSLY_DEFINED_REAL
#endif

#undef VOID
#ifdef MY_PREVIOUSLY_DEFINED_VOID
#  define VOID MY_PREVIOUSLY_DEFINED_VOID
#endif

namespace jy_mesh {
    int triangulate_polygon(const Eigen::Matrix2Xd &V,
                            const Eigen::Matrix2Xi &E,
                            const Eigen::Matrix2Xd &H,
                            double max_area,
                            double min_angle,
                            Eigen::Matrix2Xd &NV,
                            Eigen::Matrix3Xi &T,
                            Eigen::Matrix2Xi &NS) {
        std::stringstream para;
        para << "a" << max_area << "q" << min_angle << "YpzBs";//s
        const std::string &full_flags = para.str();

        // Prepare the input struct
        triangulateio in;
        in.numberofpoints = V.cols();
        in.pointlist = const_cast<double *>(V.data());

        in.numberofpointattributes = 0;
        in.pointmarkerlist = (int *) calloc(V.cols(), sizeof(int));
        std::fill(in.pointmarkerlist, in.pointmarkerlist + V.cols(), 1);

        in.trianglelist = NULL;
        in.triangleattributelist = NULL;
        in.numberoftriangles = in.numberofcorners = in.numberoftriangleattributes = 0;

        in.numberofsegments = E.cols();
        in.segmentlist = const_cast<int *>(E.data());
        in.segmentmarkerlist = (int *) calloc(E.cols(), sizeof(int));
        std::fill(in.segmentmarkerlist, in.segmentmarkerlist + E.cols(), 1);

        in.numberofholes = H.cols();
        in.holelist = const_cast<double *>(H.data());
        in.numberofregions = 0;

        // Prepare the output struct
        triangulateio out;
        out.pointlist = NULL;
        out.trianglelist = out.segmentlist = NULL;
        // Call triangulate
        ::triangulate(const_cast<char *>(full_flags.c_str()), &in, &out, 0);

        // Return the mesh
        NV.resize(2, out.numberofpoints);
        std::copy(out.pointlist, out.pointlist + NV.size(), NV.data());
        T.resize(3, out.numberoftriangles);
        std::copy(out.trianglelist, out.trianglelist + T.size(), T.data());
        NS.resize(2, out.numberofsegments);
        std::copy(out.segmentlist, out.segmentlist + NS.size(), NS.data());

        // Cleanup in & out
        free(in.pointmarkerlist);
        free(in.segmentmarkerlist);
        free(out.pointlist);
        free(out.trianglelist);
        free(out.segmentlist);
        return 0;
    }
}
