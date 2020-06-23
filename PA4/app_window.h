
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
#include "so_capsule.h"
# include "normals.h"


// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
    GlShader _vertexsh, _fragsh;
	//new shaders
	GlShader _gouraud, _smtlgouraud;
    GlProgram _prog;
	//new glprog
	GlProgram _prog2;
    // My scene objects:
    SoAxis _axis;

	// Instance of my capsule
	SoCapsule _capsule;

    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    bool  _viewaxis;
    int _w, _h;
	
	//boolean for capsule
	bool _viewcapsule;
	bool _viewnormal;

	//values to alter the shape of the capsule
	float rtChange;
	float rbChange;
	float nfacesChange;
	
	//inial values
	float rt;
	float rb;
	int nfaces;
	float len;

	//shading
	bool shadType = true;

	//lights
	float lightz;

	//normals
	normals norm;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
 };

#endif // APP_WINDOW_H
