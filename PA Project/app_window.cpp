
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
   _viewb = true;
   _viewp = true;
   _viewlc = true;
   _viewrc = true;
   _viewua = true;
   _viewla = true;
   _fovy = GS_TORAD(60.0f);
   _rotx = _roty = 0;
   _w = w;
   _h = h;
 }

void AppWindow::initPrograms ()
 {
   // Init my scene objects:
   _axis.init();
   _b.init(); 
   _p.init();
   _lc.init();
   _rc.init();
   _la.init();
   _ua.init();
   _floor.init();


   _bshadow.init();
   _pshadow.init();
   _lcshadow.init();
   _rcshadow.init();
   _lashadow.init();
   _uashadow.init();

   perspective = true;

   _sides = _fwdbck = 0.0;
   _rotB = _rotP = _rotLC = _rotRC = _rotLA= _rotUA =0.0;
   _rotfUnit = _rotLAj = _rotUAj = _rotRCj = _rotLCj =0.0;

   // set light:
   _light.set ( GsVec(0,0,10), GsColor(90,90,90,255), GsColor::white, GsColor::white );

   // Load demo model:
   loadModel(.01f);

 }

static void printInfo ( GsModel& m )
 {
   std::cout<<"V:  "<<m.V.size()<<"\n";
   std::cout<<"F:  "<<m.F.size()<<"\n";
   std::cout<<"N:  "<<m.N.size()<<"\n";
   std::cout<<"M:  "<<m.M.size()<<"\n";
   std::cout<<"Fn: "<<m.Fn.size()<<"\n";
   std::cout<<"Fm: "<<m.Fm.size()<<"\n";
 }

void AppWindow::loadModel ( float model )
 {

	GsString fileBase;
	GsString filePivot;
	GsString fileUpperArm;
	GsString fileLowerArm;
	GsString fileRightClaw;
	GsString fileLeftClaw;

	fileBase = "../models/Base.obj";
	std::cout << "Loading " << fileBase << "...\n";
	if (!_gsmBase.load(fileBase)) std::cout << "Error!\n";
	_gsmBase.scale(model); // to fit our camera space
	_b.build(_gsmBase);
				 
	fileUpperArm = "../models/UpperArm.obj";
	std::cout << "Loading " << fileUpperArm << "...\n";
	if (!_gsmUpperArm.load(fileUpperArm)) std::cout << "Error!\n";
	_gsmUpperArm.scale(model); // to fit our camera space
	_ua.build(_gsmUpperArm);

	fileLowerArm = "../models/LowerArm.obj";
	std::cout << "Loading " << fileLowerArm << "...\n";
	if (!_gsmLowerArm.load(fileLowerArm)) std::cout << "Error!\n";
	_gsmLowerArm.scale(model); // to fit our camera space
	_la.build(_gsmLowerArm);

	filePivot = "../models/Pivot.obj";
	std::cout << "Loading " << filePivot << "...\n";
	if (!_gsmPivot.load(filePivot)) std::cout << "Error!\n";
	_gsmPivot.scale(model); // to fit our camera space
	_p.build(_gsmPivot);	

	fileLeftClaw = "../models/LeftClaw.obj";
	std::cout << "Loading " << fileLeftClaw << "...\n";
	if (!_gsmLeftClaw.load(fileLeftClaw)) std::cout << "Error!\n";
	_gsmLeftClaw.scale(model); // to fit our camera space
	_lc.build(_gsmLeftClaw);

	fileRightClaw = "../models/RightClaw.obj";
	std::cout << "Loading " << fileRightClaw << "...\n";
	if (!_gsmRightClaw.load(fileRightClaw)) std::cout << "Error!\n";
	_gsmRightClaw.scale(model); // to fit our camera space
	_rc.build(_gsmRightClaw);

	_floor.build(_gsmFloor);

	_bshadow.build(_gsmBase);
	_pshadow.build(_gsmPivot);
	_lcshadow.build(_gsmLeftClaw);
	_rcshadow.build(_gsmRightClaw);
	_uashadow.build(_gsmUpperArm);
	_lashadow.build(_gsmLowerArm);

   redraw();
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
   {
	  case 27 : exit(1); // Esc was pressed
	  case ' ': perspective = !perspective; redraw(); break;
	  
	  //rot LA
	  case 'q': _rotLAj += 0.3; redraw(); break;
      case 'a' :_rotLAj -= 0.3; redraw(); break;
      
	  //rot UA
	  case 'w': _rotUAj += 0.3; redraw(); break;
	  case 's':_rotUAj -= 0.3; redraw(); break;

	  //rot LC
	  case 'e': _rotLC += 0.3; redraw(); break;
	  case 'd':_rotLC -= 0.3; redraw(); break;

	 //rot RC
	  case 'r': _rotRC += 0.3; redraw(); break;
	  case 'f':_rotRC -= 0.3; redraw(); break;

	  //rot crane
	  case 't': _rotfUnit += 0.3; redraw(); break;
      case 'g': _rotfUnit -= 0.3; redraw(); break;

	  //camera
	  case 'z': _roty -= GS_TORAD(2.5f); redraw(); break;
	  case 'x': _roty += GS_TORAD(2.5f); redraw(); break;
	  case 'c': _rotx += GS_TORAD(2.5f); redraw(); break;
	  case 'v': _rotx -= GS_TORAD(2.5f); redraw(); break;

	}
 }

void AppWindow::glutSpecial ( int key, int x, int y )
 {
   bool rd=true;
   const float incf=0.05f;
   const float incoffset = 0.01f;
   switch ( key )
    { case GLUT_KEY_LEFT:     _sides -=incoffset; break;
      case GLUT_KEY_RIGHT:    _sides += incoffset; break;
      case GLUT_KEY_UP:       _fwdbck -= incoffset; break;
      case GLUT_KEY_DOWN:     _fwdbck += incoffset; break;
      case GLUT_KEY_PAGE_UP:   _fovy-=incf; break;
      case GLUT_KEY_PAGE_DOWN: _fovy+=incf; break;
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
void AppWindow::glutDisplay()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Build a cross with some lines (if not built yet):
	if (_axis.changed) { _axis.build(1.0f); } // axis has radius 1

	//builds changes of each part of the crane
	if (_b.changed) { _b.build(_gsmBase); _b.changed = false; }
	if (_p.changed) { _p.build(_gsmPivot); _p.changed = false; }
	if (_lc.changed) { _lc.build(_gsmLeftClaw); _lc.changed = false; }
	if (_rc.changed) { _rc.build(_gsmRightClaw); _rc.changed = false; }
	if (_ua.changed) { _ua.build(_gsmUpperArm); _ua.changed = false; }
	if (_la.changed) { _la.build(_gsmLowerArm); _la.changed = false; }

	// Define our scene transformation:

	//rotations
	GsMat rx, ry;
	GsMat rxB, ryP;
	GsMat rxLA, ryLA;
	GsMat rxUA, ryUA;
	GsMat rxLC, ryLC;
	GsMat rxRC, ryRC;

	//points of rotations
	GsMat fullUnit;
	GsMat LAjoint, UAjoint;
	GsMat LCjoint, RCjoint;

	//translations
	GsMat fullUnittrans, Ptrans;
	GsMat RCtrans, LCtrans;
	GsMat UAtrans, LAtrans;
	GsMat fullUnittrans1, Ptrans1;
	GsMat RCtrans1, LCtrans1;
	GsMat UAtrans1, LAtrans1;

	//combined transformations
	GsMat stransf, stransfPivot;
	GsMat stransfRightClaw, stransfLeftClaw;
	GsMat stransfUpperArm, stransfLowerArm;

	// Define our projection transformation:
	GsMat camview, persp, sproj;

	//rotations
	rx.rotx(_rotx); ry.roty(_roty);
	fullUnit.roty(_rotfUnit);
	LAjoint.rotz(_rotLAj); rxLA.rotx(_rotLA); ryLA.roty(_rotLA);
	UAjoint.rotz(_rotUAj); rxUA.rotx(_rotUA); ryUA.roty(_rotUA);
	LCjoint.roty(_rotLCj); rxLC.rotx(_rotLC); ryLC.rotx(_rotLC);
	RCjoint.roty(_rotRCj); rxRC.rotx(_rotRC); ryRC.roty(_rotRC);
	rxB.rotx(_rotB); ryP.roty(_rotP);

	//translations
	fullUnittrans.translation(_sides, 0, _fwdbck);
	LAtrans.translation(.045 - _sides, -.075, -.043 - _fwdbck);
	UAtrans.translation(.10 - _sides, -.325, -.043 - _fwdbck);
	Ptrans.translation(.045 - _sides, -.07, -.04 - _fwdbck);
	LCtrans.translation(-.155 - _sides, -.205, -.0275 - _fwdbck);
	RCtrans.translation(-.155 - _sides, -.205, -.06 - _fwdbck);

	fullUnittrans1.translation(-_sides, 0, -_fwdbck);
	LAtrans1.translation(-.045 + _sides, .075, .043 + _fwdbck);
	UAtrans1.translation(-.10 + _sides, .325, .043 + _fwdbck);
	Ptrans1.translation(-.045 + _sides, .07, .04 + _fwdbck);
	LCtrans1.translation(.155 + _sides, .205, .0275 + _fwdbck);
	RCtrans1.translation(.155 + _sides, .205, .06 + _fwdbck);

	//fulltransformations
	stransf = rx*ry; // set the scene transformation matrix
	stransfPivot = Ptrans1 * fullUnit *Ptrans;
	stransfLowerArm = stransfPivot* LAtrans1 * LAjoint * LAtrans;
	stransfUpperArm = stransfLowerArm* UAtrans1 * UAjoint * UAtrans;
	stransfLeftClaw = stransfUpperArm* LCtrans1  *rxLC *LCtrans;
	stransfRightClaw = stransfUpperArm* RCtrans1  * rxRC * RCtrans;

	//camera perspective
	if (perspective) {
		GsVec eye(0, 0, 2), center(0, 0, 0), up(0, 1, 0);
		camview.lookat(eye, center, up); // set our 4x4 "camera" matrix
	}
	else {
		GsVec eye(_sides, 3, _fwdbck), center(_sides, 1, _fwdbck), up(1.0, 0.5, 0.0);// birds eye view
		camview.lookat(eye, center, up); // set our 4x4 "camera" matrix
	}

	float aspect = 1.0f, znear = 0.1f, zfar = 50.0f;
	persp.perspective(_fovy, aspect, znear, zfar); // set our 4x4 perspective matrix

	// Our matrices are in "line-major" format, so vertices should be multiplied on the 
	// right side of a matrix multiplication, therefore in the expression below camview will
	// affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
	sproj = persp * camview; // set final scene projection

	GsMat shadow;
	GsVec v, n, q;
	float vn, qn;

	v.set(0.0f, 0.5f, 0.5f);
	q.set(0.0f, 0.01f, 0.0f);
	n.set(0, 1, 0);

	//magnitude of v
	v = v / sqrt(v.x *v.x + v.y*v.y + v.z*v.z);

	//dot product of vn and qn
	vn = v.x*n.x + v.y*n.y + v.z*n.z;
	qn = q.x*n.x + q.y*n.y + q.z*n.z;

	//the actual matrix of shadow
	//top left 3x3 matrix within the overall 4x4 shadow matrix
	shadow.setl1(GsVec(vn, 0.0f, 0.0f) - GsVec(0.0f, v.x*n.y, 0.0f));
	shadow.setl2(GsVec(0.0f, vn, 0.0f) - GsVec(0.0f, v.y*n.y, 0.0f));
	shadow.setl3(GsVec(0.0f, 0.0f, vn) - GsVec(0.0f, v.z*n.y, 0.0f));
	//the bottom row of the matrix
	shadow.setl4(0.0f, 0.0f, 0.0f, vn);

	//the far right column of the matrix
	shadow[3] = (qn)*v.x;
	shadow[7] = (qn)*v.y;
	shadow[11] = (qn)*v.z;
	shadow[15] = vn;

	//  Note however that when the shader receives a matrix it will store it in column-major 
	//  format, what will cause our values to be transposed, and we will then have in our 
	//  shaders vectors on the left side of a multiplication to a matrix.

	// Draw:
	if (_viewaxis) _axis.draw(stransf, sproj);

	if (_viewb) {
		_b.draw(stransf/*stransfPivot*fullUnittrans*/, sproj, _light);
		//_bshadow.draw(stransf*shadow*stransfPivot*fullUnittrans, sproj, _light);
	//}

		if (_viewp) {
			_p.draw(stransf*stransfPivot*fullUnittrans, sproj, _light);
			_pshadow.draw(stransf*shadow*stransfPivot*fullUnittrans, sproj, _light);
		}

		if (_viewla) {
			_la.draw(stransf*stransfLowerArm*fullUnittrans, sproj, _light);
			_lashadow.draw(stransf*shadow*stransfLowerArm*fullUnittrans, sproj, _light);
		}

		if (_viewua) {
			_uashadow.draw(stransf*shadow*stransfUpperArm*fullUnittrans, sproj, _light);
			_ua.draw(stransf*stransfUpperArm*fullUnittrans, sproj, _light);
		}
		if (_viewlc) {
			_lcshadow.draw(stransf*shadow*stransfLeftClaw*fullUnittrans, sproj, _light);
			_lc.draw(stransf*stransfLeftClaw*fullUnittrans, sproj, _light);
		}

		if (_viewrc) {
			_rcshadow.draw(stransf*shadow*stransfRightClaw*fullUnittrans, sproj, _light);
			_rc.draw(stransf*stransfRightClaw*fullUnittrans, sproj, _light);
		}

		_floor.draw(stransf, sproj, _light);


		// Swap buffers and draw:
		glFlush();         // flush the pipeline (usually not necessary)
		glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
	}
}


