#ifndef CURVE_EVAL_H
#define CURVE_EVAL_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"


	void eval_bezier(float segments, GsArray<GsVec>& actcurve, const GsArray<GsVec>& ctrlpnts);
	GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts);


#endif // CURVE_EVAL_H
