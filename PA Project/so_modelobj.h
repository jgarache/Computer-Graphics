// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODELOBJ_H
#define SO_MODELOBJ_H


// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include <gsim/gs_image.h>
# include <gsim/gs_mat.h>
# include <gsim/gs_light.h>
# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_model.h>
# include "ogl_tools.h"
# include <cmath>

#include "light.h"
#include "material.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object capsule:
class SoModelObj : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	std::vector<GsVec> N; // normals
	std::vector<GsVec2> T; // Texture coords

	gsuint _texid;

	int _numpoints;         // saves the number of points
	GsMaterial _mtl;    // main material
	bool _phong;
	GlShader _vshgou, _fshgou, _vshphong, _fshphong;
	GlProgram _proggouraud, _progphong;

public:
	SoModelObj();
	void phong(bool b) { _phong = b; }
	bool phong() const { return _phong; }
	void init();
	void build(GsModel& m);
	void draw(const GsMat& tr, const GsMat& pr, const GsLight& l);
};

#endif // SO_AXIS_H
