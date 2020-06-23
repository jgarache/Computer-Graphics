
# include <iostream>
# include "ogl_tools.h"
# include "app_window.h"

//==========================================================================
// Main routine
//==========================================================================
int main ( int argc, char** argv )
 {
   // Init freeglut library:
   glutInit ( &argc, argv );
   glutInitDisplayMode ( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
   glutInitContextProfile ( GLUT_CORE_PROFILE );
 
   // Now create the window of your application:
   AppWindow* w = new AppWindow ( "CSE-170 Support Code", 300, 300, 640, 480 );

   // Finally start the main loop:
   w->run ();
}
