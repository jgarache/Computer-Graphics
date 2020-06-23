// Ensure the header file is included only once in multi-file projects
#ifndef normals_model_H
#define normals_model_H


// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>

#include "light.h"
#include "material.h"
#include "model.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object capsule:
class normals_model : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	std::vector<GsVec> N; // normals_model
	int _numpoints;         // saves the number of points
	Model type;

public:
	normals_model();
	void init(const GlProgram& prog, char* armPart );
	void build(bool shadType);
	void draw ( GsMat& tr, GsMat& pr);
	
};

#endif // SO_AXIS_H
