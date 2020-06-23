#include "normals_model.h"

normals_model::normals_model()
{
	_numpoints = 0;
}

void normals_model::init(const GlProgram& prog, char* armPart)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");

	//loads each part of the arm
	type.load(armPart);

}

void normals_model::build(bool shadType)
{

	double scale = 45.0;

	for (int i = 0; i < type.fsize; i++) {
		GsVec ver = (type.V[type.F[i].va]) / scale;
		GsVec ver1 = (type.V[type.F[i].vb]) / scale;
		GsVec ver2 = (type.V[type.F[i].vc]) / scale;

		if (shadType) {
			//flat
			GsVec mid = (ver + ver1 + ver2) / 3;
			P.push_back(mid);
			GsVec a(type.N[type.F[i].na]*.01);
			GsVec b((type.N[type.F[i].nb] * .01));
			GsVec c((type.N[type.F[i].nc] * .01));

			P.push_back(((a + b + c) / 3) + mid);

		}
		else {
			//smooth
			P.push_back(ver);
			P.push_back(ver + (type.N[type.F[i].na] * .01));
			
			P.push_back(ver1);
			P.push_back(ver1 + (type.N[type.F[i].nb] * .01));

			P.push_back(ver2);
			P.push_back(ver2 + (type.N[type.F[i].nc] * .01));

		}
	}

	for (int i = 0; i < P.size(); i++) {
		C.push_back(GsColor::white);
	}

	// send data to OpenGL buffers:
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	
	//glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
	//glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	
	glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
	glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = P.size();

	// free non-needed memory:
	P.resize(0); C.resize(0); N.resize(0);
}

void normals_model::draw(GsMat& tr, GsMat& pr)
{
	// Draw Lines:
	glUseProgram(prog);
	glBindVertexArray(va[0]);

	glBindBuffer(GL_ARRAY_BUFFER, buf[0]); // positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // colors
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);

	glUniformMatrix4fv(uniloc[0], 1, GL_FALSE, tr.e);
	glUniformMatrix4fv(uniloc[1], 1, GL_FALSE, pr.e);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_LINES, 0, _numpoints);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	

	//draw the things
}
