// Ensure the header file is included only once in multi-file projects
#ifndef SO_CAPSULE_H
#define SO_CAPSULE_H


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
class SoCapsule : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	int _numpoints;         // saves the number of points

							//added
	std::vector<GsVec> N; // normals

public:
	SoCapsule();
	void init(const GlProgram& prog);
	void build(float len, float rt, float rb, int nfaces, bool shadType);
	//void draw ( GsMat& tr, GsMat& pr);
	//changed to below
	void draw(GsMat& tr, GsMat& pr, light& light, mat& material);
};

#endif // SO_AXIS_H
