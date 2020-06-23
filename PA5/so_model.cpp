#include "so_model.h"
#include <iostream>


SoModel::SoModel()
 {
   _numpoints = 0;
 }

void SoModel::init ( const GlProgram& prog, char* armPart, mat& material)
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 10 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );

   //lab 4
   //loading shaders
   uniform_location(2, "lPos");
   uniform_location(3, "la");
   uniform_location(4, "ld");
   uniform_location(5, "ls");
   uniform_location(6, "ka");
   uniform_location(7, "kd");
   uniform_location(8, "ks");
   uniform_location(9, "sh");

   //loads each part of the arm
   type.load(armPart);
   this->material = material;

 }

//lab 4
//builds the shadded capsule
void SoModel::build  (bool shadType )
 {

	double scale = 45.0;

	for (int i = 0; i < type.fsize; i++) {
		P.push_back((type.V[type.F[i].va]) / scale);
		P.push_back((type.V[type.F[i].vb]) / scale);
		P.push_back((type.V[type.F[i].vc]) / scale);

		if (shadType) {
			//flat
			GsVec a(type.N[type.F[i].na]);
			GsVec b((type.N[type.F[i].nb]));
			GsVec c((type.N[type.F[i].nc]));

			N.push_back((a+b+c) / 3);
			N.push_back((a + b + c) / 3);
			N.push_back((a + b + c) / 3);

		}
		else {
			//smooth
			N.push_back((type.N[type.F[i].na]));
			N.push_back((type.N[type.F[i].nb]));
			N.push_back((type.N[type.F[i].nc]));

		}
	}

   // send data to OpenGL buffers:
   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, P.size()*3*sizeof(float), &P[0], GL_STATIC_DRAW );
   glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
   glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
//   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
//   glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );

   // save size so that we can free our buffers and later just draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0); N.resize(0);
 }



void SoModel::draw ( GsMat& tr, GsMat& pr, light& light)
 {
   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );


   glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // norm
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
 /*  glBindBuffer ( GL_ARRAY_BUFFER, buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );*/

   glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

   float f[4]; // we convert below our color values to be in [0,1]
   glUniform3fv(uniloc[2], 1, light.pos.e);
   glUniform4fv(uniloc[3], 1, light.amb.get(f));
   glUniform4fv(uniloc[4], 1, light.dif.get(f));
   glUniform4fv(uniloc[5], 1, light.spe.get(f));
   glUniform4fv(uniloc[6], 1, material.amb.get(f));
   glUniform4fv(uniloc[7], 1, material.dif.get(f));
   glUniform4fv(uniloc[8], 1, material.spe.get(f));
   glUniform1fv(uniloc[9], 1, &material.sh);

   //Changed 
   //glDrawArrays(GL_LINES,0,_numpoint);
   //to The following b/c I'm using triangles in wireframe mode


   /*/Wireframe
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glDrawArrays(GL_TRIANGLES,0,_numpoints);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);*/

   //Real
   glDrawArrays(GL_TRIANGLES, 0, _numpoints);
 }

