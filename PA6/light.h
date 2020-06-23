#ifndef SO_LIGHT_H
#define SO_LIGHT_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

class light : public GlObjects {
public:
	GsVec pos;
	GsColor amb, dif, spe;
	light(const GsVec& p, const GsColor& a, const GsColor& d, const GsColor& s)
		:pos(p), amb(a), dif(d), spe(s) {}
};
#endif // SO_LIGHT_H

