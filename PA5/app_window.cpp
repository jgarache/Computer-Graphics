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
   _viewmodel = true;
   _viewnormal = true;

   //inital values
   lightz = 0.0f;
   wrist = shoulder = elbow = 0.0;

 }

void AppWindow::initPrograms ()
 {
	// Load your shaders and link your programs here:
	_vertexsh.load_and_compile(GL_VERTEX_SHADER, "../3dcolored.vert");
	_smtlgouraud.load_and_compile(GL_VERTEX_SHADER, "../smtlgouraud.vert");//added

	_fragsh.load_and_compile(GL_FRAGMENT_SHADER, "../flat.frag");
	_gouraud.load_and_compile(GL_FRAGMENT_SHADER, "../gouraud.frag");	//added

	_prog.init_and_link(_vertexsh, _fragsh);
	_prog2.init_and_link(_smtlgouraud, _gouraud); //added

   // Init my scene objects:
   _axis.init ( _prog );
	
   //init normal vectors
   norm.init(_prog,"rhand.m");
   norm1.init(_prog, "rlowerarm.m");
   norm2.init(_prog, "rupperarm.m");

   mat materialHand(GsColor::yellow, GsColor::yellow, GsColor::white, 1);
   mat materialLowerarm(GsColor::darkblue, GsColor::darkblue, GsColor::white, 1);
   mat materialUpperarm(GsColor::darkblue, GsColor::darkblue, GsColor::white, 1);

   rhand.init(_prog2, "rhand.m", materialHand);
   rlowerarm.init(_prog2, "rlowerarm.m", materialLowerarm);
   rupperarm.init(_prog2, "rupperarm.m", materialUpperarm);


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
   switch ( key )
    { case ' ': _viewaxis = !_viewaxis; redraw(); break;
	  case 27 : exit(1); // Esc was pressed
		
	  // defining what q, a, w, s, e, d key will do when pressed
	  //shoulder
	  //q - up
	  //a - down
	  case 'q': shoulder -= 0.05f; redraw(); break;
	  case 'a': shoulder += 0.05f; redraw(); break;
	  
   	  //elbow
      //w - up
	  //s - down
	  case 'w': elbow -= 0.05f; redraw(); break;
	  case 's': elbow += 0.05f; redraw(); break;

	  //hand
	  //e - up
	  //d - down
	  case 'e': wrist -= 0.05; redraw(); break;
	  case 'd': wrist += 0.05; redraw(); break;

	  //turns normals on/off normals
	  case 'v': _viewnormal = !_viewnormal; redraw(); break;

	  //z - wireframe
	  //x - flat
	  //c - smooth
	  case 'z': shadType = false; glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); redraw(); break; 
	  case 'x': shadType = true; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); redraw(); break;
	  case 'c': shadType = false; glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); redraw(); break;
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

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay ()
 {
   // Clear the rendering window
   glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   // Build a cross with some lines (if not built yet):
   if ( _axis.changed ) // needs update
    { _axis.build(1.0f); // axis has radius 1.0
    }

   //builds arm parts
   if(rhand.changed){
	   rhand.build(shadType);
	   norm.build(shadType);
   }
   if (rlowerarm.changed) {
	   rlowerarm.build(shadType);
	   norm1.build(shadType);
   }
   if (rupperarm.changed) {
	   rupperarm.build(shadType);
	   norm2.build(shadType);
   }

   // Define our scene transformation:
   GsMat rx, ry, stransf;
   //new variables
   GsMat stransfHand, stransfLowerarm, stransfUpperarm, //transformation
	   handtrans, upperarmtrans, lowerarmtrans, //translation
	   _rotwrist, _rotshoulder, _rotelbow; //rotation

   //creates translation matrix
   handtrans.translation(0,0,0.6);
   lowerarmtrans.translation(0, 0, 0.6);
   upperarmtrans.translation(0, 0, -0.6);


   //creates rotatoin matrix
   _rotwrist.rotx(wrist);
   _rotshoulder.rotx(shoulder);
   _rotelbow.rotx(elbow);

   //overall matrix with transformations applied to them
   stransfUpperarm = upperarmtrans*_rotshoulder;
   stransfLowerarm = stransfUpperarm * lowerarmtrans * _rotelbow;
   stransfHand = stransfLowerarm * handtrans *_rotwrist;

   rx.rotx ( _rotx );
   ry.roty ( _roty );
   stransf = rx*ry; // set the scene transformation matrix


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

   light light(GsVec(0.0f, 1.0f, 1.0f + lightz), GsColor::gray, GsColor::gray, GsColor::gray);
   mat material(GsColor::darkblue, GsColor::yellow, GsColor::white, 1);

   // Draw:
   if ( _viewaxis ) _axis.draw ( stransf, sproj );

   //added build model if changed, later draw

   if (_viewnormal) norm.draw(stransf*stransfHand, sproj);
   if (_viewnormal) norm1.draw(stransf*stransfLowerarm, sproj);
   if (_viewnormal) norm2.draw(stransf*stransfUpperarm, sproj);

   //stransf1 = stransf * rot_shoulder * upperarmtrans * rot_joint * handtrans * rot_arm;
   if (_viewpart) rhand.draw(stransf*stransfHand, sproj, light);

   if (_viewpart) rlowerarm.draw(stransf*stransfLowerarm, sproj, light);

   if (_viewpart) rupperarm.draw(stransf*stransfUpperarm, sproj, light);



   // Swap buffers and draw:
   glFlush();         // flush the pipeline (usually not necessary)
   glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

