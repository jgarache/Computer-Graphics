#ifndef MATERIAL_H
#define MATERIAL_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

class mat : public GlObjects {

public:
	GsColor amb, dif, spe;
	GsVec pos;
	float sh;

	mat(const GsColor& a, const GsColor& d, const GsColor& s, float sn)
		: amb(a), dif(d), spe(s), sh(sn) {}

	mat() {};
};
#endif // SO_LIGHT_H

