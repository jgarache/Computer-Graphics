
# include "so_axis.h"

SoAxis::SoAxis()
 {
   _numpoints = 0;
 }

void SoAxis::init ( const GlProgram& prog )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
 }

void SoAxis::build ( float r )
 {
   int i;
   const float d=r/20.0f;
   
   P.clear(); C.clear(); // set size to zero, just in case
   P.reserve(18); C.reserve(18); // reserve space to avoid re-allocations from the calls below

   P.push_back( GsVec( -r,  0, 0 ) ); P.push_back( GsVec( r, 0, 0 ) );
   P.push_back( GsVec( r-d,-d, 0 ) ); P.push_back( GsVec( r, 0, 0 ) );
   P.push_back( GsVec( r-d, d, 0 ) ); P.push_back( GsVec( r, 0, 0 ) );
   for ( i=0; i<6; i++ ) C.push_back( GsColor::red ); // recall that GsColor has r,g,b,a values

   P.push_back( GsVec( 0, -r,  0 ) ); P.push_back( GsVec( 0, r, 0 ) );
   P.push_back( GsVec( 0, r-d,-d ) ); P.push_back( GsVec( 0, r, 0 ) );
   P.push_back( GsVec( 0, r-d, d ) ); P.push_back( GsVec( 0, r, 0 ) );
   for ( i=0; i<6; i++ ) C.push_back( GsColor::green );

   glColor3f ( 0, 0, 1 );		
   P.push_back( GsVec( 0, 0,  -r ) ); P.push_back( GsVec( 0, 0, r ) );
   P.push_back( GsVec( 0,-d, r-d ) ); P.push_back( GsVec( 0, 0, r ) );
   P.push_back( GsVec( 0, d, r-d ) ); P.push_back( GsVec( 0, 0, r ) );
   for ( i=0; i<6; i++ ) C.push_back( GsColor::blue );

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

void SoAxis::draw ( GsMat& tr, GsMat& pr )
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

   glDrawArrays ( GL_LINES, 0, _numpoints );
 }

