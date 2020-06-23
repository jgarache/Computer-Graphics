// Ensure the header file is included only once in multi-file projects
#ifndef SO_CURVE_H
#define SO_CURVE_H

// Include needed header files
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "so_poly.h"
# include "curve_eval.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object axis:
class SoCurve : public GlObjects
{
private:
	GsArray<GsVec>   V; // Curveline coordinates
	GsArray<GsColor> C; // colors
	GsArray<GsVec> N;
	int _numpoints;
	
	bool _threeD;
	GlMaterial mat;
	float shine;
	

public:
	SoCurve();
	void init(const GlProgram& prog, const GlMaterial&mat, float shine);
	void build(SoPoly& poly, float segs, bool& threeD, bool curveType);
	void draw(GsMat& tr, GsMat& pr, const GlLight& light);
};

#endif // SO_CURVE_H
