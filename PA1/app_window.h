
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <vector>
# include <gsim/gs_color.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private : // My OGL Data
    GlShader _vertexsh, _fragsh;
    GlProgram _prog;
    GlObjects _tris, _pts, _lines;
	std::vector<GsVec>   _tricoords;
    std::vector<GsColor> _tricolors;
    std::vector<GsVec>   _ptcoords;
    std::vector<GsColor> _ptcolors;
    std::vector<GsVec>   _linecoords;
    std::vector<GsColor> _linecolors;

   private : // App Data
    enum MenuEv { evOption0, evOption1 };
    GsColor _markc;
    GsVec2 _mark;
    int _w, _h;
	double pacX = 0;
	double pacX2 = 0;
	GsVec moveBottom;
	GsVec moveTop;

	GsVec moveBottom2;
	GsVec moveTop2;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
	void buildObjects ();
    GsVec2 windowToScene ( const GsVec2& v );

   private : // functions derived from the base class
    virtual void glutMenu ( int m );
    virtual void glutKeyboard ( unsigned char key, int x, int y );
    virtual void glutSpecial ( int key, int x, int y );
    virtual void glutMouse ( int b, int s, int x, int y );
    virtual void glutMotion ( int x, int y );
    virtual void glutDisplay ();
    virtual void glutReshape ( int w, int h );
	virtual void glutIdle();
 };

#endif // APP_WINDOW_H
