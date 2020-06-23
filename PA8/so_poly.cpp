# include "so_poly.h"
# include "curve_eval.h"

SoPoly::SoPoly()
 {
   _uptodate = false;
   _selection = -1;
   _selr = 0.1f;
   _polysize = _fullsize = 0;
 }

void SoPoly::init ( const GlProgram& prog, const GsColor& c, const GsColor& selc )
 {
   // Define buffers needed:
   set_program ( prog );
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 2 );       // will use 2 buffers: one for coordinates and one for colors
   uniform_locations ( 2 ); // will send 2 variables: the 2 matrices below
   uniform_location ( 0, "vTransf" );
   uniform_location ( 1, "vProj" );
   _color = c;
   _selcolor = selc;
 }

void SoPoly::add ( const GsVec& p, int pos )
 {
   _uptodate = false;
   if ( _selection>=V.size() ) _selection=-1;

   int sel = _selection;
   if ( sel<V.size() ) // there is a selection, add there
    { int inc = 1;
      if ( sel>0 ) // choose where to add
       { int nsel = sel<V.size()-1? sel+1:sel;
         if ( dist(p,V[sel-1])<dist(p,V[nsel]) ) inc=0;
       }
      pos = sel+inc;
      _selection += inc;
    } 

   if ( pos<0 )
    V.push() = p;
   else
    V.insert ( pos ) = p;
 }

void SoPoly::move ( int pos, const GsVec& p )
 {
   if ( pos<0 || pos>=V.size() ) return;
   _uptodate = false;
   V[pos] = p;
 }

void SoPoly::del ( int pos )
 {
   if ( pos<0 || pos>=V.size() ) return;
   _uptodate = false;
   V.remove ( pos );
   if ( _selection>0 && _selection>=V.size() ) _selection--;
 }

int SoPoly::selection ()
 { 
   if ( _selection>=V.size() ) _selection=-1;
   return _selection;
 }

void SoPoly::select ( int i, float r )
 {
   //std::cout<<"Selected "<<i<<"\n"<<gsnl;
   _uptodate = false;
   _selection = i;
   _selr = r;
 }

void SoPoly::make_selection ()
 {
   int i = _selection;
   _polysize = V.size();
   if ( i<0 || i>=V.size() ) return; // there is no selection
   GsVec c = V[i];
   int n=12;
   float a=0, da = (2*3.1415f)/n;
   while ( n-->=0 )
    { V.push().set ( c.x+_selr*cos(a), c.y+_selr*sin(a), 0 );
      C.push() = _selcolor;
      a+=da;
    }
 }

void SoPoly::curveEval(float segs, int curveType, GsArray<GsVec>& actCurve)
{
	
	if (V.size() > 0) {
		if(curveType < 3) actCurve.push(V[0]);
		if (curveType >= 4) actCurve.push(V[1]);

		for (int i = 1; i < segs; i++) {
			if (curveType == 1) {

				//std::cout << "lagrange"<<std::endl;
				float t = (V.size() - 1)* (i/ segs);
				actCurve.push(eval_lagrange(t,V));

			}
			else if (curveType == 2) {
				//std::cout << "bezier"<<std::endl;
				float t = i / segs;
				actCurve.push(eval_bezier(t, V));
			}
			else if (curveType == 3){
				//std::cout << "bspline"<<std::endl;
				float t = (float) (V.size()- 2) * (i / segs) + 2;
				actCurve.push(eval_bspline(t, 3, V));
					
			}
			else if (curveType == 4){
				//std::cout << "catmull" << std::endl;
				float t = (V.size() - 3)* (i / segs);
				actCurve.push(eval_crspline(t, V));
			}
			else if (curveType == 5){
				//std::cout << "bessel" << std::endl;
				float t = (float)i / segs;
				actCurve.push(eval_bospline(t, V));
			}
		}
		if(curveType < 3) actCurve.push(V[V.size() - 1]);
		if (curveType == 4) actCurve.push(V[V.size() - 2]);
	}	
}

void SoPoly::draw ( GsMat& tr, GsMat& pr )
 {
   // make sure latest buffer was sent:
   if ( !_uptodate )
    { V.size ( V.size() );
      C.setall ( _color );
      make_selection ();

      // send data to OpenGL buffers:
      glBindVertexArray ( va[0] );
      glEnableVertexAttribArray ( 0 );
      glEnableVertexAttribArray ( 1 );

      glBindBuffer ( GL_ARRAY_BUFFER, buf[0] );
      glBufferData ( GL_ARRAY_BUFFER, V.size()*3*sizeof(float), V.pt(), GL_STATIC_DRAW );
      glVertexAttribPointer ( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

      glBindBuffer ( GL_ARRAY_BUFFER, buf[1] );
      glBufferData ( GL_ARRAY_BUFFER, C.size()*4*sizeof(gsbyte), C.pt(), GL_STATIC_DRAW );
      glVertexAttribPointer ( 1, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0 );

      _uptodate = true;
      _fullsize = V.size();
      V.size ( _polysize );
    }

   // Draw Lines:
   glUseProgram ( prog );
   glBindVertexArray ( va[0] );
   glUniformMatrix4fv ( uniloc[0], 1, GL_FALSE, tr.e );
   glUniformMatrix4fv ( uniloc[1], 1, GL_FALSE, pr.e );

   glDrawArrays ( GL_POINTS, 0, _polysize );
   glDrawArrays ( GL_LINE_STRIP, 0, _polysize );
   glDrawArrays ( GL_LINE_STRIP, _polysize, _fullsize-_polysize );
   glBindVertexArray ( 0 );
 }


