
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"

AppWindow::AppWindow ( const char* label, int x, int y, int w, int h )
          :GlutWindow ( label, x, y, w, h )
 {
   initPrograms ();
   addMenuEntry ( "Option 0", evOption0 );
   addMenuEntry ( "Option 1", evOption1 );
   _viewaxis = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;

   //setting bool caps to true
   _viewstopWatch = true;
   reset = 1;
   oldtime = 0;
   pause = false;
   flashx = flashy = flashz = 0;
   
 }

void AppWindow::initPrograms ()
 {
   // Load your shaders and link your programs here:
   _vertexsh.load_and_compile ( GL_VERTEX_SHADER, "../3dcolored.vert" );
   _fragsh.load_and_compile ( GL_FRAGMENT_SHADER, "../flat.frag" );
   _prog.init_and_link ( _vertexsh, _fragsh );

   // Init my scene objects:
   _axis.init ( _prog );

   // Init capsule
   _stopWatch.init(_prog);
 }

// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene ( const GsVec2& v )
 {
   // GsVec2 is a lighteweight class suitable to return by value:
   return GsVec2 ( (2.0f*(v.x/float(_w))) - 1.0f,
                    1.0f - (2.0f*(v.y/float(_h))) );
 }

// Called every time there is a window event
void AppWindow::glutKeyboard ( unsigned char key, int x, int y )
 {
	const float incr = GS_TORAD(0.4f);

   switch ( key )
    { case 27 : exit(1); // Esc was pressed
		
	  // defining what q, a, w, s, e, d key will do when pressed
	case 'q': _stopWatch.changed = true; flashx += incr; redraw(); break; //+x
	case 'w': _stopWatch.changed = true; flashy += incr; redraw(); break; //+y
	case 'e': _stopWatch.changed = true; flashz += incr; redraw(); break; //+z
	
	case 'a': _stopWatch.changed = true; flashx -= incr; redraw(); break; //-x
	case 's': _stopWatch.changed = true; flashy -= incr;  redraw(); break;//-y
	case 'd': _stopWatch.changed = true; flashz -= incr; redraw(); break;//-z
	  
	  //added
	  case ' ': _stopWatch.changed = true; pause = !pause; redraw(); std::cout << " "<<std::endl; break;//space is to pause
	  case 13: _stopWatch.changed = true; reset = 0; redraw(); break; //enter is to reset
	  case 'p': _viewaxis = !_viewaxis; redraw(); break; // turns axis off
	  case '/': _viewshadow = !_viewshadow; redraw(); break; //turns shadow off

	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incr=GS_TORAD(2.5f);
   const float incf=0.05f;
   switch ( key )
    { case GLUT_KEY_LEFT:      _roty-=incr; break;
      case GLUT_KEY_RIGHT:     _roty+=incr; break;
      case GLUT_KEY_UP:        _rotx-=incr; break;
      case GLUT_KEY_DOWN:      _rotx+=incr; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
      case GLUT_KEY_HOME:      _fovy=GS_TORAD(60.0f); _rotx=_roty=0; break;
      default: return; // return without rendering
	}
   if (rd) redraw(); // ask the window to be rendered when possible
 }

void AppWindow::glutMouse ( int b, int s, int x, int y )
 {
   // The mouse is not used in this example.
   // Recall that a mouse click in the screen corresponds
   // to a whole line traversing the 3D scene.
 }

void AppWindow::glutMotion ( int x, int y )
 {
 }

void AppWindow::glutMenu ( int m )
 {
   std::cout<<"Menu Event: "<<m<<std::endl;
 }

void AppWindow::glutReshape ( int w, int h )
 {
   // Define that OpenGL should use the whole window for rendering
   glViewport( 0, 0, w, h );
   _w=w; _h=h;
 }

void AppWindow::glutIdle()
{
	
	if(pause) oldtime = glutGet(GLUT_ELAPSED_TIME)*.001 - time;

	time = glutGet(GLUT_ELAPSED_TIME)*.001;
	

	if (reset == 0) {

		oldtime = glutGet(GLUT_ELAPSED_TIME)*.001;
	
		reset = 1;
	}

	time -= oldtime;

	redraw();
}

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   //build the stopwatch
   if(_stopWatch.changed)
   {
	   _stopWatch.build(time);
   }


   // Define our scene transformation:
   GsMat rx, ry, stransf;
   GsMat shadow;
   GsMat trans;

   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix

   rx.rotx(GS_TORAD(-90.0f));

   trans.translation(0.0f,-0.5f,-0.5f);
   shadow = trans*rx;


   // Define our projection transformation:
   // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
   GsMat camview, persp, sproj;

   GsVec eye(0,0,2), center(0,0,0), up(0,1,0);
   camview.lookat ( eye, center, up ); // set our 4x4 "camera" matrix

   float aspect=1.0f, znear=0.1f, zfar=50.0f;
   persp.perspective ( _fovy, aspect, znear, zfar ); // set our 4x4 perspective matrix

   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
   // right side of a matrix multiplication, therefore in the expression below camview will
   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
   sproj = persp * camview; // set final scene projection
   //  Note however that when the shader receives a matrix it will store it in column-major 
   //  format, what will cause the values to be transposed, and we will then have in our 
   //  shaders vectors on the left side of a multiplication to a matrix.

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );

   //added build capsule if changed, later draw
   if (_viewstopWatch) _stopWatch.draw(stransf, sproj );
  
   //the shadow
   //follows the formula from the lecture slide
   GsVec v, n, q;
   float vn, qn;

   v.set(0.0f+flashx,0.5f+flashy,0.5f+flashz);
   q.set(0.0f, -0.5f, -0.5f);
   n.set(0,1,0);

   //magnitude of v
   v = v / sqrt(v.x *v.x + v.y*v.y + v.z*v.z);
   
   //dot product of vn and qn
   vn = v.x*n.x + v.y*n.y+v.z*n.z;
   qn = q.x*n.x + q.y*n.y + q.z*n.z;

   //the actual matrix of shadow
   //top left 3x3 matrix within the overall 4x4 shadow matrix
   shadow.setl1(GsVec(vn, 0.0f,0.0f)-GsVec(0.0f,v.x*n.y,0.0f));
   shadow.setl2(GsVec(0.0f, vn, 0.0f)-GsVec(0.0f,v.y*n.y,0.0f));
   shadow.setl3(GsVec(0.0f, 0.0f, vn)-GsVec(0.0f,v.z*n.y,0.0f));
   //the bottom row of the matrix
   shadow.setl4(0.0f, 0.0f, 0.0f, vn);

   //the far right column of the matrix
   shadow[3] = (qn)*v.x;
   shadow[7] = (qn)*v.y;
   shadow[11] = (qn)*v.z;
   shadow[15] = vn;

   /*
   setl1 ( p[0],  p[1],  p[2],  p[3]  );
   setl2 ( p[4],  p[5],  p[6],  p[7]  );
   setl3 ( p[8],  p[9],  p[10], p[11] );
   setl4 ( p[12], p[13], p[14], p[15] );
   */

   //if (_viewshadow) _stopWatch.draw(shadow*stransf, sproj);
  if (_viewshadow) _stopWatch.draw(stransf*shadow, sproj); //fixated shadow\
  
   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

