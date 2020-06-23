# include "so_curve.h"
# include "so_poly.h"
# include "curve_eval.h"

SoCurve::SoCurve()
 {
	threeDSurface = false;
 }

void SoCurve::init ( const GlProgram& prog, const GlMaterial&mat, float shine)
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 10 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
   uniform_location(2, "lPos");
   uniform_location(3, "la");
   uniform_location(4, "ld");
   uniform_location(5, "ls");
   uniform_location(6, "ka");
   uniform_location(7, "kd");
   uniform_location(8, "ks");
   uniform_location(9, "sh");

   this->mat = mat;
   this->shine = shine;
 }

void SoCurve::build(SoPoly & poly, float segs, bool& threeDSwitch, int curveType)
{
	V.remove(0, V.size());

	poly.curveEval(segs, curveType, V);

	GsVec nerm;
	for (int i = 0; i < V.size(); i++) {
		nerm = V[i];
		nerm.normalize();
		N.push(nerm);
	}

	if (threeDSwitch) {
		if (threeDSurface) {
			//Turns on/off updates
			threeDSwitch = false;
			changed = false;
		}
		else {
			//turns on 3D
			threeDSurface = true;
		}
	}

	if (threeDSurface) {
		GsArray<GsVec> newNorm, newVert;
		
		GsVec Z = GsVec(0.0f, 0.0f, -0.25f);
		for (int i = 0; i < V.size() - 1; i++) {
			newVert.push(V[i + 1] + Z);		newNorm.push(N[i + 1]);
			newVert.push(V[i] + Z);			newNorm.push(N[i]);
			newVert.push(V[i]);				newNorm.push(N[i]);
			newVert.push(V[i]);				newNorm.push(N[i]);
			newVert.push(V[i + 1]);			newNorm.push(N[i + 1]);
			newVert.push(V[i + 1] + Z);		newNorm.push(N[i + 1]);
		}
		V = newVert;
		N = newNorm;
	}
	_numpoints = V.size();
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, V.size() * 3 * sizeof(float), &V[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);


}

void SoCurve::draw ( GsMat& tr, GsMat& pr, const GlLight& light )
 {

	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);


	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // norm
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	/*  glBindBuffer ( GL_ARRAY_BUFFER, buf[1] ); // colors
	glEnableVertexAttribArray ( 1 );
	glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );*/

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	float f[4]; // we convert below our color values to be in [0,1]
	glUniform3fv(uniloc[2], 1, light.pos);
	glUniform4fv(uniloc[3], 1, light.ambient);
	glUniform4fv(uniloc[4], 1, light.diffuse);
	glUniform4fv(uniloc[5], 1, light.specular);
	glUniform4fv(uniloc[6], 1, mat.ambient);
	glUniform4fv(uniloc[7], 1, mat.diffuse);
	glUniform4fv(uniloc[8], 1, mat.specular);
	glUniform1fv(uniloc[9], 1, &shine);

	//Real
	if(threeDSurface)
		glDrawArrays(GL_TRIANGLES, 0, _numpoints);
	else
		glDrawArrays ( GL_LINE_STRIP, 0, _numpoints );
   glBindVertexArray ( 0 );
 }

