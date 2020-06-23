
// Ensure the header file is included only once in multi-file projects
#ifndef SO_POLY_H
#define SO_POLY_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "curve_eval.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoPoly : public GlObjects
 { private :
    GsArray<GsVec>   V; // polyline coordinates
    GsArray<GsColor> C; // colors
    GsColor _color, _selcolor;
    int _selection;
    int _polysize, _fullsize;
    float _selr;
    bool _uptodate;
    void make_selection ();

   public :
    SoPoly ();
    void changed () { _uptodate=false; }
    const GsArray<GsVec>& vertexarray() { return V; }
    GsVec* vertices() { return V.pt(); }
    int num_vertices () const { return V.size(); }
    void add ( const GsVec& p, int pos=-1 );
    void move ( int pos, const GsVec& p );
    void del ( int pos );
    int selection ();
    void select ( int i, float r );
    void init ( const GlProgram& prog, const GsColor& c, const GsColor& selc );
    void draw ( GsMat& tr, GsMat& pr );
	void curveEval(float segs, bool mode, GsArray<GsVec>& actCurve);
 };

#endif // SO_POLY_H
