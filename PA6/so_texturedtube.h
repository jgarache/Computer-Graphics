// Ensure the header file is included only once in multi-file projects
#ifndef SO_TEXTUREDTUBE_H
#define SO_TEXTUREDTUBE_H


// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>

#include "light.h"
#include "material.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object capsule:
class SoTexturedtube : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	std::vector<GsVec> N; // normals
	std::vector<GsVec2> T; // Texture coords

	gsuint _texid;
	gsuint _texid2;

	int _numpoints;         // saves the number of points

public:
	SoTexturedtube();
	void init(const GlProgram& prog);
	void build(float len, float rt, float rb, int nfaces, bool shadType);
	//void draw ( GsMat& tr, GsMat& pr);
	//changed to below
	void draw(GsMat& tr, GsMat& pr, light& light, mat& material, float shad,bool texType);
};

#endif // SO_AXIS_H
