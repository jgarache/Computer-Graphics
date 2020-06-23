#ifndef CURVE_EVAL_H
#define CURVE_EVAL_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"

	//lab7
	GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts);
	GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts);

	//lab8
	GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts);
	GsVec eval_crspline(float t, const GsArray<GsVec>& pnts);
	GsVec eval_bospline(float t, const GsArray<GsVec>& pnts);
	static float N(int i, int k, float u);
	



#endif // CURVE_EVAL_H
