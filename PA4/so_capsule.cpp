# include "so_capsule.h"
#include <iostream>


SoCapsule::SoCapsule()
 {
   _numpoints = 0;
 }

void SoCapsule::init ( const GlProgram& prog )
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
 }

void SoCapsule::build ( float len, float rt, float rb, int nfaces, bool shadType )
 {
   //const float d=r/20.0f;
   
   P.clear(); C.clear(); // set size to zero, just in case
   P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

   
   double length = len / 2; //length of the tube (up and down the y axis)
   double  PI = 3.1415926535897; //setting pi to a variable
   double thetaIncremented = ((2 * PI) / nfaces); // change of theta through each incrementation
   double phiIncremented = (PI / (2 * nfaces)); //change in phi through each incrementation

   GsVec v1, v2, v3; //vertices for the triangles

   if (shadType) {
	   for (double theta = 0; theta < (2 * PI) - 0.001; theta += thetaIncremented) {
		   //out of bounds so take into consideration the rounding errors
		   //top tube radius
		   v3 = GsVec(rt*cos(theta), length, rt*sin(theta)); //top left
		   v2 = GsVec(rb*cos((theta)), -length, rb*sin((theta))); //bottom left
		   v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented))); //top right	  
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   for (int i = 0; i<3; i++) C.push_back(GsColor::red);

		   //bottom tube radius
		   v3 = GsVec(rb*cos(theta), -length, rb*sin(theta)); //bottom left
		   v2 = GsVec(rb*cos((theta + thetaIncremented)), -length, rb*sin((theta + thetaIncremented)));// bottom right
		   v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented)));//top right
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::red);

		   //top lid
		   v1 = GsVec(rt*cos(theta), length, rt*sin(theta)); //
		   v2 = GsVec(0.0, length, 0.0); //
		   v3 = GsVec(rt*cos(theta + thetaIncremented), length, rt*sin(theta + thetaIncremented));//
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);

		   //bottom lid
		   v3 = GsVec(rt*cos(theta), -length, rt*sin(theta));//
		   v2 = GsVec(0.0, -length, 0.0);//inside
		   v1 = GsVec(rt*cos(theta + thetaIncremented), -length, rt*sin(theta + thetaIncremented));//
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));

		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);
	   }

   }
   else {
	   for (double theta = 0; theta < (2 * PI) - 0.001; theta += thetaIncremented) {
		   //out of bounds so take into consideration the rounding errors
		   //top tube radius
		   v3 = GsVec(rt*cos(theta), length, rt*sin(theta)); //top left
		   v2 = GsVec(rb*cos((theta)), -length, rb*sin((theta))); //bottom left
		   v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented))); //top right	  

		   GsVec nv3 = v3 - GsVec(0.0,length,0.0);
		   GsVec nv2 = v2 - GsVec(0.0, length, 0.0);
		   GsVec nv1 = v1 - GsVec(0.0, length, 0.0);;
		   
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);

		   nv1.normalize();
		   nv2.normalize();
		   nv3.normalize();

		   N.push_back(nv1);
		   N.push_back(nv2);
		   N.push_back(nv3);
		   for (int i = 0; i<3; i++) C.push_back(GsColor::red);

		   //bottom tube radius
		   v3 = GsVec(rb*cos(theta), -length, rb*sin(theta)); //bottom left
		   v2 = GsVec(rb*cos((theta + thetaIncremented)), -length, rb*sin((theta + thetaIncremented)));// bottom right
		   v1 = GsVec(rt*cos((theta + thetaIncremented)), length, rt*sin((theta + thetaIncremented)));//top right
		   
		   nv3 = v3 - GsVec(0.0, length, 0.0);
		   nv2 = v2 - GsVec(0.0, length, 0.0);
		   nv1 = v1 - GsVec(0.0, length, 0.0);;

		   P.push_back(v1); P.push_back(v2); P.push_back(v3);

		   nv1.normalize();
		   nv2.normalize();
		   nv3.normalize();

		   N.push_back(nv1);
		   N.push_back(nv2);
		   N.push_back(nv3);
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::red);

		   //top lid
		   v1 = GsVec(rt*cos(theta), length, rt*sin(theta)); //
		   v2 = GsVec(0.0, length, 0.0); //
		   v3 = GsVec(rt*cos(theta + thetaIncremented), length, rt*sin(theta + thetaIncremented));//
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);

		   //bottom lid
		   v3 = GsVec(rt*cos(theta), -length, rt*sin(theta));//
		   v2 = GsVec(0.0, -length, 0.0);//inside
		   v1 = GsVec(rt*cos(theta + thetaIncremented), -length, rt*sin(theta + thetaIncremented));//
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));
		   N.push_back(normal(v1, v2, v3));

		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);
	   }
   }

 



   /*if (shadType) {
	   for (int i = 0; i < N.size() - 12; i += 12) {
		   GsVec mid;
		   mid = N[i + 0] + N[i + 12];	mid.normalize();
		   N[i + 2] = mid;	N[i + 4] = mid; N[i + 5] = mid;
		   if (i == 0) {
			   mid = N[i + 0] + N[N.size() - 12];	mid.normalize();
		   }
		   else {
			   mid = N[i + 0] + N[i - 12];	mid.normalize();
		   }
		   N[i + 0] = mid;	N[i + 1] = mid; N[i + 3] = mid;

	   }
   }*/

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

void SoCapsule::draw ( GsMat& tr, GsMat& pr, light& light, mat& material)
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

