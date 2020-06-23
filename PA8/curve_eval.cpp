# include "curve_eval.h"
# include "vector"
# include "math.h"

GsVec eval_bezier(float t, const GsArray<GsVec>& ctrlpnts)
{
	GsArray<GsVec> temp, save = ctrlpnts;

	for (int i = 0; i < ctrlpnts.size(); i++) {
		temp.remove(0, temp.size());

		for (int j = 0; j < save.size() - 1; j++) {
			temp.push(save[j]+(save[j+1]-save[j])*t);
		}
		save = temp;
	}
	//std::cout << save[0] << std::endl;
	return save[0];
}

GsVec eval_lagrange(float t, const GsArray<GsVec>& ctrlpnts)
{
	float y = 0;
	GsVec result(0,0,0);
	float numerator;
	float denominator;

	for (int i = 0; i < ctrlpnts.size(); i++) {
		numerator = 1.0f;
		denominator = 1.0f;
		for (int j = 0; j < ctrlpnts.size(); j++) {
			if (j != i) {
				numerator *= t - j;
				denominator *= i - j;
			}
		}
		result += ctrlpnts[i]*(numerator / denominator);
	}

	return result;
}

GsVec eval_bspline(float t, int k, const GsArray<GsVec>& pnts)
{
	GsVec result(0, 0, 0);
	for (int i = 0; i < pnts.size(); i++) {
		result += pnts[i] * N(i, k, t);
	}
	return result;
}

static float N(int i, int k, float u) {
	float ui = float(i);
	if (k == 1)
		return ui <= u && u < ui + 1 ? 1.0f : 0;
	else
		return ((u - ui) / (k - 1))*N(i, k - 1, u)
		+ ((ui + k - u) / (k - 1)) * N(i + 1, k - 1, u);
}

GsVec eval_crspline(float t, const GsArray<GsVec>& pnts)
{	
	GsVec result(0,0,0);
	int i = (int)t + 1;
	t -= (int)t;

	result = 0.5 *((2 * pnts[i]) +
		(-pnts[i-1] + pnts[i+1]) * t +
		(2 * pnts[i-1] - 5 * pnts[i] + 4 * pnts[i+1] - pnts[i+2]) * (t*t) +
		(-pnts[i-1] + 3 * pnts[i] - 3 * pnts[i+1] + pnts[i+2]) * (t*t*t));

	return result;
}

GsVec eval_bospline(float t, const GsArray<GsVec>& pnts)
{	
	GsArray<GsVec> result;
	GsVec dIneg, dIpos, vIneg, vIpos, pIneg, pIpos,vI;
	float distNeg, distPos;

	result.push(pnts[0]);
	for (int i = 1; i < pnts.size() - 1; i++) {
		dIneg = pnts[i] - pnts[i - 1];
		dIpos = pnts[i + 1] - pnts[i];

		distNeg = dIneg.norm();
		distPos = dIpos.norm();

		vIpos = (pnts[i + 1] - pnts[i]) / distPos;
		vIneg = (pnts[i] - pnts[i - 1]) / distNeg;

		vI = (distPos*vIneg + distNeg*vIpos) / (distPos + distNeg);

		pIneg = pnts[i] - (distNeg*vI/3);
		pIpos = pnts[i] + (distPos*vI/3);

		result.push(pIneg);
		result.push(pnts[i]);
		result.push(pIpos);
	}
	result.push(pnts[pnts.size()-1]);
	t = t * (result.size() - 2) + 2;
	 return eval_bspline(t, 3,result);
}

