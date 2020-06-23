# include "normals.h"

normals::normals()
{
	_numpoints = 0;
}

void normals::init(const GlProgram& prog)
{
	// Define buffers needed:
	set_program(prog);
	gen_vertex_arrays(1); // will use 1 vertex array
	gen_buffers(2);       // will use 2 buffers: one for coordinates and one for colors
	uniform_locations(2); // will send 2 variables: the 2 matrices below
	uniform_location(0, "vTransf");
	uniform_location(1, "vProj");
}

void normals::build(float len, float rt, float rb, int nfaces, bool shadType)
{

	P.clear(); C.clear(); // set size to zero, just in case
	P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below


	double length = len / 2; //length of the tube (up and down the y axis)
	double  PI = 3.1415926535897; //setting pi to a variable
	double thetaIncremented = ((2 * PI) / nfaces); // change of theta through each incrementation
	double phiIncremented = (PI / (2 * nfaces)); //change in phi through each incrementation

	GsVec v1, v2, v3; //vertices for the triangles
	GsVec mid, norm;

	for (double theta = 0; theta < (2 * PI) - 0.001; theta += thetaIncremented) {
		
		//top tube radius
		v3 = GsVec(rt*cos(theta), length, rt*sin(theta)); //top left
		v2 = GsVec(rb*cos((theta)), -length, rb*sin((theta))); //bottom left
		v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented))); //top right	  
		norm = normal(v1, v2, v3)*0.1f;
		if (shadType) {
			N.push_back(v1);  N.push_back(v1 + norm);//  N.push_back(v2);  N.push_back(v2 + norm);  N.push_back(v3);  N.push_back(v3 + norm);
			for (int i = 0; i < 6; i++)
				C.push_back(GsColor::white);
		}
		else {
			mid = (v1 + v2 + v3) / 3;
			N.push_back(mid);
			N.push_back(mid + norm);
			for (int i = 0; i < 2; i++)
				C.push_back(GsColor::white);
		}

		//bottom tube radius
		v3 = GsVec(rb*cos(theta), -length, rb*sin(theta)); //bottom left
		v2 = GsVec(rb*cos((theta + thetaIncremented)), -length, rb*sin((theta + thetaIncremented)));// bottom right
		v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented)));//top right
		norm = normal(v1, v2, v3)*0.1f;
		if (shadType) {
			N.push_back(v1);  N.push_back(v1 + norm);  N.push_back(v2);  N.push_back(v2 + norm);//  N.push_back(v3);  N.push_back(v3 + norm);
			for (int i = 0; i < 6; i++)
				C.push_back(GsColor::white);
		}
		else {
			mid = (v1 + v2 + v3) / 3;
			N.push_back(mid);
			N.push_back(mid + norm);
			for (int i = 0; i < 2; i++)
				C.push_back(GsColor::white);
		}

		//top lid
		v1 = GsVec(rt*cos(theta), length, rt*sin(theta)); //
		v2 = GsVec(0.0, length, 0.0); //
		v3 = GsVec(rt*cos(theta + thetaIncremented), length, rt*sin(theta + thetaIncremented));//
		norm = normal(v1, v2, v3)*0.1f;
		if (shadType) {
			N.push_back(v1);  N.push_back(v1 + norm);//  N.push_back(v2);  N.push_back(v2 + norm);  N.push_back(v3);  N.push_back(v3 + norm);
			for (int i = 0; i < 6; i++)
				C.push_back(GsColor::white);
		}
		else {
			mid = (v1 + v2 + v3) / 3;
			N.push_back(mid);
			N.push_back(mid + norm);
			for (int i = 0; i < 2; i++)
				C.push_back(GsColor::white);
		}

		//bottom lid
		v3 = GsVec(rt*cos(theta), -length, rt*sin(theta));//
		v2 = GsVec(0.0, -length, 0.0);//inside
		v1 = GsVec(rt*cos(theta + thetaIncremented), -length, rt*sin(theta + thetaIncremented));//
		norm = normal(v1, v2, v3)*0.1f;
		if (shadType) {
			N.push_back(v1);  N.push_back(v1 + norm);  //N.push_back(v2);  N.push_back(v2 + norm);  N.push_back(v3);  N.push_back(v3 + norm);
			for (int i = 0; i < 6; i++)
				C.push_back(GsColor::white);
		}
		else {
			mid = (v1 + v2 + v3) / 3;
			N.push_back(mid);
			N.push_back(mid + norm);
			for (int i = 0; i < 2; i++)
				C.push_back(GsColor::white);
		}
	

	}
		
	// send data to OpenGL buffers:
	//glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	//glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
	glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
	
	glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
    glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );

	// save size so that we can free our buffers and later just draw the OpenGL arrays:
	_numpoints = N.size();

	// free non-needed memory:
	P.resize(0); C.resize(0); N.resize(0);
}

void normals::draw(GsMat& tr, GsMat& pr)
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
