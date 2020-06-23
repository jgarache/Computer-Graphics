# include "curve_eval.h"
# include "vector"



void eval_bezier(float segments, GsArray<GsVec>& actcurve, const GsArray<GsVec>& ctrlpnts)
{
	if (ctrlpnts.size() > 2) {
		actcurve = ctrlpnts;
		GsArray<GsVec> temp;
		for (int i = 0; i < segments; i++) {
			temp.remove(0, temp.size());
			temp.push(actcurve[0]);
			for (int j = 0; j < actcurve.size() - 1; j++) {
				temp.push((actcurve[j] + actcurve[j + 1]) / 2);
			}
			temp.push(actcurve[actcurve.size() - 1]);
			actcurve = temp;
		}
	}
}

GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts)
{
	float y = 0;
	float term =0 ;
	float numerator;
	float denominator;

	for (int i = 0; i < ctrlpnts.size(); i++) {
		numerator = 1.0f;
		denominator = 1.0f;
		for (int j = 0; j < ctrlpnts.size(); j++) {
			if (j != i) {
				numerator *= t - ctrlpnts[j].x;
				denominator *= ctrlpnts[i].x - ctrlpnts[j].x;
			}
		}
		term = ctrlpnts[i].y*(numerator / denominator);
		y += term;
	}


	return GsVec(t,y, 0.0f);
}

