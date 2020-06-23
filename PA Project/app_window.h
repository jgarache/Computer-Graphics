// Ensure the header file is includ only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "so_modelobj.h"
//#include "so_texturedtube.h"

// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
 { private :
    // OpenGL shaders and programs:
//    GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
  //  GlProgram _progflat, _proggou, _progphong;

    // My scene objects:
    SoAxis _axis;
	SoModel _b;
	SoModel _lc;
	SoModel _rc;
	SoModel _la;
	SoModel _ua;
	SoModel _p;
	SoModelObj _floor;

	SoModel _bshadow;
	SoModel _lcshadow;
	SoModel _rcshadow;
	SoModel _lashadow;
	SoModel _uashadow;
	SoModel _pshadow;

    // Scene data:
    bool  _viewaxis;
	bool  _viewb;
	bool  _viewp;
	bool  _viewlc;
	bool  _viewrc;
	bool  _viewua;
	bool  _viewla;
	bool perspective;
    GsLight _light;
	
	GsModel _gsmBase;
	GsModel _gsmLeftClaw;
	GsModel _gsmRightClaw;
	GsModel _gsmLowerArm;
	GsModel _gsmUpperArm;
	GsModel _gsmPivot;
	GsModel _gsmFloor;

	float _sides, _fwdbck;
	float _rotB, _rotP, _rotLC, _rotRC, _rotLA, _rotUA;
	float _rotfUnit, _rotLAj, _rotUAj, _rotRCj, _rotLCj;

    // App data:
    enum MenuEv { evOption0, evOption1 };
    float _rotx, _roty, _fovy;
    int _w, _h;

   public :
    AppWindow ( const char* label, int x, int y, int w, int h );
    void initPrograms ();
    void loadModel ( float model );
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
