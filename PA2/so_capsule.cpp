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
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoCapsule::build ( float len, float rt, float rb, int nfaces )
 {
   //const float d=r/20.0f;
   
   P.clear(); C.clear(); // set size to zero, just in case
   P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

   
   double length = len / 2; //length of the tube (up and down the y axis)
   double  PI = 3.1415926535897; //setting pi to a variable
   double thetaIncremented = ((2 * PI) / nfaces); // change of theta through each incrementation
   double phiIncremented = (PI / (2 * nfaces)); //change in phi through each incrementation

   for (double theta = 0; theta < (2 * PI)- 0.001; theta += thetaIncremented) { //controls the up and down shift on y axis
	   for (double phi = 0; phi < (PI/2)-0.001; phi+= phiIncremented) {//controsl left and right shift around the y axis
		   //ent out of bounds so take into consideration the rounding errors


		  // std::cout << "t: " << theta << ", p: " << phi << std::endl;

		   // x  = cos(theta)
		   // z = sin(theta)
		   //y = distance the center of the base circle is at 
		   //i.e the location of the top radius and bottom radius

		   //top tube radius
		   P.push_back(GsVec(rt*cos(theta), length, rt*sin(theta))); //top left
		   P.push_back(GsVec(rb*cos((theta)), -length, rb*sin((theta)))); //bottom left

		   P.push_back(GsVec(rt*cos((theta+thetaIncremented)), length, rt*sin((theta+thetaIncremented)))); //top right	  
		   for (int i = 0; i<3; i++) C.push_back(GsColor::orange);

		   //bottom tube radius
		   P.push_back(GsVec(rb*cos(theta), -length, rb*sin(theta)));

		   P.push_back(GsVec(rb*cos((theta+ thetaIncremented)), -length, rb*sin((theta+ thetaIncremented))));
		   P.push_back(GsVec(rt*cos((theta+ thetaIncremented)), length, rt*sin((theta+ thetaIncremented))));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::orange);

		   //x = r * cos(theta) * sin(phi)
		   //z = r * sin(theta) * sin(phi)
		   //y = r * cos(phi)

		   //theta is controlling the top down shifts
		   //phi is controlling the left right shifts 

		   //top dome
		   P.push_back(GsVec(rt*cos(theta)*sin(phi), length+rt*cos(phi), rt*sin(theta)*sin(phi))); //top left
		   P.push_back(GsVec(rt*cos(theta+ thetaIncremented)*sin(phi), length+ rt*cos(phi), rt*sin(theta+thetaIncremented)*sin(phi)));//top right
		   P.push_back(GsVec(rt*cos(theta + thetaIncremented)*sin(phi+ phiIncremented), length + rt*cos(phi + phiIncremented), rt*sin(theta + thetaIncremented)*sin(phi+ phiIncremented)));//bottom left
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);

		   //bottom dome
		   P.push_back(GsVec(rb*cos(theta)*sin(phi), -1 * (length + rb*cos(phi)), rb*sin(theta)*sin(phi)));
		   P.push_back(GsVec(rb*cos(theta + thetaIncremented)*sin(phi), -1 * (length + rb*cos(phi)), rb*sin(theta + thetaIncremented)*sin(phi)));
		   P.push_back(GsVec(rb*cos(theta + thetaIncremented)*sin(phi + phiIncremented), -1 * (length + rb*cos(phi + phiIncremented)), rb*sin(theta + thetaIncremented)*sin(phi + phiIncremented)));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);

	   }
   }
  // std::cout << std::endl;

   // send data to OpenGL buffers:
   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, P.size()*3*sizeof(float), &P[0], GL_STATIC_DRAW );
   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
   glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );

   // save size so that we can free our buffers and later just draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0);
 }

void SoCapsule::draw ( GsMat& tr, GsMat& pr )
 {
   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] ); // colors
   glEnableVertexAttribArray ( 1 );
   glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

   glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

   //Changed 
   //glDrawArrays(GL_LINES,0,_numpoint);
   //to The following b/c I'm using triangles in wireframe mode

   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glDrawArrays(GL_TRIANGLES,0,_numpoints);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
 }

