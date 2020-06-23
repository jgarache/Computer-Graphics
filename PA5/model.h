
// Ensure the header file is included only once in multi-file projects
// Pragma once is a non-standard but widely supported preprocessor directive
# pragma once

# include <gsim/gs_vec.h>

// a class to store a model with triangular faces
class Model
 { public :
    struct Fac { int va, vb, vc; // vertices indices of the (triangular) face
                 int na, nb, nc; // normal indices
                 int r, g, b; // r g b color
               };

    GsVec* V; // vertex table
    GsVec* N; // normals table
    Fac* F; // faces table
    int vsize, nsize, fsize; // sizes of the tables

   public :

    /*! constructor */
    Model () { vsize=nsize=fsize=0; V=N=0; F=0; }

    /*! destructor */
   ~Model () { delete[] V; delete[] N; delete[] F; }

    /*! load model */
    void load ( const char* file );
 };

