// Ensure the header file is included only once in multi-file projects
#ifndef SO_MODEL_H
#define SO_MODEL_H


// Include needed header files
# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include <cmath>

//lab 4
#include "light.h"
#include "material.h"

//lab 5
#include "model.h"

// Scene objects should be implemented in their own classes; and
// here is an example of how to organize a scene object in a class.
// Scene object MODEL:
class SoModel : public GlObjects
{
private:
	std::vector<GsVec>   P; // coordinates
	std::vector<GsColor> C; // colors
	int _numpoints;         // saves the number of points

							//added
	std::vector<GsVec> N; // normals
	mat material;

public:
	SoModel();
	void init(const GlProgram& prog, char* armPart, mat& material);
	void build(bool shadType);
	//void draw ( GsMat& tr, GsMat& pr);
	//changed to below
	void draw(GsMat& tr, GsMat& pr, light& light);
	Model type;

};

#endif // SO_AXIS_H
