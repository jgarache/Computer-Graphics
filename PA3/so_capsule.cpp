# include "so_capsule.h"
#include <iostream>

SoStopWatch::SoStopWatch()
 {
   _numpoints = 0;
 }

void SoStopWatch::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }



void SoStopWatch::build (double time)
 {
   
   P.clear(); C.clear(); // set size to zero, just in case
   P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

   double circle = GS_TORAD(360.0f); //acts as PI
   double shiftTheta = GS_TORAD(6.0f); //shift in theta. GS_TORAD converts degrees -> radians
   int bigTicks = 0;
   int bigTicksInc = 6;


   for (double theta = 0; theta < circle; theta += shiftTheta) { //controls the up and down shiftTheta on y axis
	 // x  = cos(theta)
	// z = sin(theta)
	//y = distance the center of the base circle is at
	   
	   
	   //out circle
	   P.push_back(GsVec(0.5*sin(theta), 0.5*cos(theta) ,0.0)); //top left
	   P.push_back(GsVec(0.5*sin(theta), 0.5*cos(theta), 0.0)); //bottom left
	   P.push_back(GsVec(0.5*sin((theta + shiftTheta)), 0.5*cos((theta + shiftTheta)) ,0.0)); //top right	  
	   for (int i = 0; i<3; i++) C.push_back(GsColor::white);

	   //ticks
	   if (bigTicks % 30 == 0) {
		   P.push_back(GsVec(0.5*sin(theta), 0.5*cos(theta), 0.0));
		   P.push_back(GsVec(0.5*sin((theta)), 0.5*cos((theta)), 0.0));
		   P.push_back(GsVec(0.4*sin(theta), 0.4*cos(theta), 0.0));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);
	   }
	   else {
		   P.push_back(GsVec(0.5*sin(theta), 0.5*cos(theta), 0.0));
		   P.push_back(GsVec(0.5*sin((theta)), 0.5*cos((theta)), 0.0));
		   P.push_back(GsVec(0.45*sin(theta), 0.45*cos(theta), 0.0));
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::white);
	   } bigTicks+=bigTicksInc;
   }

   //big hand (minute)
   P.push_back(GsVec(0.0, 0.0, 0.0));
   P.push_back(GsVec(0.0, 0.0, 0.0));
   P.push_back(GsVec(.5*sin(shiftTheta*60*time), .5*cos(shiftTheta*60*time), 0.0));
   for (int i = 0; i < 3; i++) C.push_back(GsColor::blue);

   //small hand (seconds)
   P.push_back(GsVec(0.0, 0.0, 0.0));
   P.push_back(GsVec(0.0, 0.0, 0.0));
   P.push_back(GsVec(.3*sin(shiftTheta*(int)time), (.3*cos(shiftTheta*(int)time)), 0.0)); //it ticks :D
   for (int i = 0; i < 3; i++) C.push_back(GsColor::red);

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

void SoStopWatch::draw ( GsMat& tr, GsMat& pr )
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

