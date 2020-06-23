# include "so_texturedtube.h"
# include <gsim/gs_image.h>
#include <iostream>


SoTexturedtube::SoTexturedtube()
 {
   _numpoints = 0;
 }

void SoTexturedtube::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 3 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 11 ); // will send 2 variables: the 2 matrices below
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
   uniform_location(10, "Tex1");
   uniform_location(11, "shadInfluence");

   //provided code to initalize calls to load texture

   GsImage I;

   if (!I.load("../Pokeball.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
   glGenTextures(1, &_texid); // generated ids start at 1
   glBindTexture(GL_TEXTURE_2D, _texid);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindVertexArray(0);
   I.init(0, 0); // free image from CPU

   if (!I.load("../Mew.png")) { std::cout << "COULD NOT LOAD IMAGE!\n"; exit(1); }
   glGenTextures(1, &_texid2); // generated ids start at 1
   glBindTexture(GL_TEXTURE_2D, _texid2);
   glTexImage2D(GL_TEXTURE_2D, 0, 4, I.w(), I.h(), 0, GL_RGBA, GL_UNSIGNED_BYTE, I.data());
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
   glGenerateMipmap(GL_TEXTURE_2D);
   glBindTexture(GL_TEXTURE_2D, 0);
   glBindVertexArray(0);
   I.init(0, 0); // free image from CPU
 }

void SoTexturedtube::build ( float len, float rt, float rb, int nfaces, bool texType)
 {
   
   P.clear(); C.clear(); // set size to zero, just in case
   P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

   
   double length = len / 2; //length of the tube (up and down the y axis)
   double  PI = 3.1415926535897; //setting pi to a variable
   double thetaIncremented = ((2 * PI) / nfaces); // change of theta through each incrementation
   double phiIncremented = (PI / (2 * nfaces)); //change in phi through each incrementation
   float n = (float)nfaces;
   float i = 0;
   GsVec v1, v2, v3; //vertices for the triangles

	   for (double theta = 0; theta < (2 * PI); theta += thetaIncremented) {
		   
		   //top tube radius
		   v1 = GsVec(rt*sin(theta), length, rt*cos(theta)); //top left
		   v2 = GsVec(rb*sin((theta)), -length, rb*cos((theta))); //bottom left
		   v3 = GsVec(rt*sin((theta + thetaIncremented)), length, rt*cos((theta + thetaIncremented))); //top right	  

		   //normal vector
		   GsVec nv3 = v3 - GsVec(0.0, length,0.0);
		   GsVec nv2 = v2 - GsVec(0.0, length, 0.0);
		   GsVec nv1 = v1 - GsVec(0.0, length, 0.0);
		   
		   //saves into p
		   P.push_back(v1); P.push_back(v2); P.push_back(v3);
		   //std::cout << (i + 1) / n << std::endl;

		   //saves into t
		   T.push_back(GsVec2(((i) / n), 0.0f)); //v1
		   T.push_back(GsVec2(((i)/ n), 1.0f)); // v2
		   T.push_back(GsVec2(((i+1)/ n), 0.0f)); //v3
		   
		   //normalizes unit vec
		   nv1.normalize();
		   nv2.normalize();
		   nv3.normalize();

		   //saves as normals
		   N.push_back(nv1);
		   N.push_back(nv2);
		   N.push_back(nv3);
		   for (int i = 0; i<3; i++) C.push_back(GsColor::red);

		   //bottom tube radius
		   v1 = GsVec(rb*sin(theta), -length, rb*cos(theta)); //bottom left
		   v2 = GsVec(rb*sin((theta + thetaIncremented)), -length, rb*cos((theta + thetaIncremented)));// bottom right
		   v3 = GsVec(rt*sin((theta + thetaIncremented)), length, rt*cos((theta + thetaIncremented)));//top right
		   
		   nv3 = v3 - GsVec(0.0, length, 0.0);
		   nv2 = v2 - GsVec(0.0, length, 0.0);
		   nv1 = v1 - GsVec(0.0, length, 0.0);

		   P.push_back(v1); P.push_back(v2); P.push_back(v3);

		   T.push_back(GsVec2(((i) / n), 1.0f)); //v1
		   T.push_back(GsVec2(((i+1) / n), 1.0f)); // v2
		   T.push_back(GsVec2(((i+1)/n), 0.0f)); //v3
		   

		   nv1.normalize();
		   nv2.normalize();
		   nv3.normalize();

		   N.push_back(nv1);
		   N.push_back(nv2);
		   N.push_back(nv3);
		   for (int i = 0; i < 3; i++) C.push_back(GsColor::red);

		   i++;
	   }

   // send data to OpenGL buffers:
   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
   glBufferData ( GL_ARRAY_BUFFER, P.size()*3*sizeof(float), &P[0], GL_STATIC_DRAW );
   
   glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
   glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
   
   glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
   glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
   //glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization
//   glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
//   glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), &C[0], GL_STATIC_DRAW );


   // save size so that we can free our buffers and later just draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0); N.resize(0);
 }

void SoTexturedtube::draw ( GsMat& tr, GsMat& pr, light& light, mat& material, float shad,bool texType)
 {
   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );
   //glBindTexture(GL_TEXTURE_2D, CurrentTexID);

   glBindBuffer ( GL_ARRAY_BUFFER, buf[0] ); // positions
   glEnableVertexAttribArray ( 0 );
   glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );


   glBindBuffer(GL_ARRAY_BUFFER, buf[1]); // norm
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

   //moved from build to here
   glBindBuffer(GL_ARRAY_BUFFER, buf[2]); // norm
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0); // false means no normalization

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
   glUniform1fv(uniloc[11], 1, &shad);

   //Changed 
   //glDrawArrays(GL_LINES,0,_numpoint);
   //to The following b/c I'm using triangles in wireframe mode

   if (texType) {
	   glBindTexture(GL_TEXTURE_2D, _texid);
   }
   else {
	   glBindTexture(GL_TEXTURE_2D, _texid2);
   }


   /*/Wireframe
   glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
   glDrawArrays(GL_TRIANGLES,0,_numpoints);
   glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);*/

   //Real
   glDrawArrays(GL_TRIANGLES, 0, _numpoints);
   glBindVertexArray(0);
 }

