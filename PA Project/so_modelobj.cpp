
# include "so_modelobj.h"

SoModelObj::SoModelObj()
 {
   _numpoints = 0;
   _phong = false;
 }

void SoModelObj::init ()
 {
   // Load programs:
   _vshgou.load_and_compile ( GL_VERTEX_SHADER, "../mcol_gouraud.vert" );
   _fshgou.load_and_compile ( GL_FRAGMENT_SHADER, "../gouraud.frag" );
   _proggouraud.init_and_link ( _vshgou, _fshgou );

   // Define buffers needed:
   gen_vertex_arrays ( 1 ); // will use 1 vertex array
   gen_buffers ( 3 );       // will use 3 buffers

   _proggouraud.uniform_locations ( 10 ); // will send 9 variables
   _proggouraud.uniform_location ( 0, "vTransf" );
   _proggouraud.uniform_location ( 1, "vProj" );
   _proggouraud.uniform_location ( 2, "lPos" );
   _proggouraud.uniform_location ( 3, "la" );
   _proggouraud.uniform_location ( 4, "ld" );
   _proggouraud.uniform_location ( 5, "ls" );
   _proggouraud.uniform_location ( 6, "ka" );
   _proggouraud.uniform_location ( 7, "ks" );
   _proggouraud.uniform_location ( 8, "sh" );
   _proggouraud.uniform_location(9, "Tex1");

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

 }

void SoModelObj::build ( GsModel& m )
{

	GsVec v1, v2, v3; //vertices for the triangles

		//furthest away from camera aka left triangle
		v1 = GsVec(1, 0, 1); //top right
		v2 = GsVec(-1, 0, 1); //top left
		v3 = GsVec(-1,0,-1); //bottom left

		//normal vector
		GsVec nv1 = v1 - GsVec(0.0, 1.0, 0.0);
		GsVec nv2 = v2 - GsVec(0.0, 1.0, 0.0);
		GsVec nv3 = v3 - GsVec(0.0, 1.0, 0.0);
		
		//saves into p
		P.push_back(v1); P.push_back(v2); P.push_back(v3);
		//std::cout << (i + 1) / n << std::endl;

		//saves into t
		T.push_back(GsVec2(1.0f, 1.0f)); //v1
		T.push_back(GsVec2(-1.0f, 1.0f)); // v2
		T.push_back(GsVec2(-1.0f, -1.0f)); //v3

		//normalizes unit vec
		nv1.normalize();
		nv2.normalize();
		nv3.normalize();

		//saves as normals
		N.push_back(nv1);
		N.push_back(nv2);
		N.push_back(nv3);
		for (int i = 0; i<3; i++) C.push_back(GsColor::red);

		//closest to the camera aka right triangle
		v1 = GsVec(1,0,1); //bottom left
		v2 = GsVec(-1, 0, -1);// bottom right
		v3 = GsVec(1, 0, -1);//top right

		nv3 = v3 - GsVec(0.0, 1.0, 0.0);
		nv2 = v2 - GsVec(0.0, 1.0, 0.0);
		nv1 = v1 - GsVec(0.0, 1.0, 0.0);

		P.push_back(v1); P.push_back(v2); P.push_back(v3);

		T.push_back(GsVec2(1.0f, 1.0f)); //v1
		T.push_back(GsVec2(-1.0f, -1.0f)); // v2
		T.push_back(GsVec2(1.0f,-1.0f)); //v3

		nv1.normalize();
		nv2.normalize();
		nv3.normalize();

		N.push_back(nv1);
		N.push_back(nv2);
		N.push_back(nv3);
		for (int i = 0; i < 3; i++) C.push_back(GsColor::red);


   // send data to OpenGL buffers:
   glBindVertexArray ( va[0] );
   glEnableVertexAttribArray ( 0 );
   glEnableVertexAttribArray ( 1 );
   glEnableVertexAttribArray ( 2 );

   // send data to OpenGL buffers:
   glBindBuffer(GL_ARRAY_BUFFER, buf[0]);
   glBufferData(GL_ARRAY_BUFFER, P.size() * 3 * sizeof(float), &P[0], GL_STATIC_DRAW);
   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buf[1]);
   glBufferData(GL_ARRAY_BUFFER, N.size() * 3 * sizeof(float), &N[0], GL_STATIC_DRAW);
   glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

   glBindBuffer(GL_ARRAY_BUFFER, buf[2]);
   glBufferData(GL_ARRAY_BUFFER, T.size() * 2 * sizeof(float), &T[0], GL_STATIC_DRAW);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
   std::cout<<"build ok.\n";

   // save size so that we can free our buffers and later draw the OpenGL arrays:
   _numpoints = P.size();

   // free non-needed memory:
   P.resize(0); C.resize(0); N.resize(0);
 }

void SoModelObj::draw ( const GsMat& tr, const GsMat& pr, const GsLight& l )
 {
   float f[4]; 
   float sh = (float)_mtl.shininess;
   if ( sh<0.001f ) sh=64;


	  glUseProgram ( _proggouraud.id );
      glUniformMatrix4fv ( _proggouraud.uniloc[0], 1, GL_FALSE, tr.e );
      glUniformMatrix4fv ( _proggouraud.uniloc[1], 1, GL_FALSE, pr.e );
      glUniform3fv ( _proggouraud.uniloc[2], 1, l.pos.e );
      glUniform4fv ( _proggouraud.uniloc[3], 1, l.amb.get(f) );
      glUniform4fv ( _proggouraud.uniloc[4], 1, l.dif.get(f) );
      glUniform4fv ( _proggouraud.uniloc[5], 1, l.spe.get(f) );
      glUniform4fv ( _proggouraud.uniloc[6], 1, _mtl.ambient.get(f) );
      glUniform4fv ( _proggouraud.uniloc[7], 1, _mtl.specular.get(f) );
      glUniform1fv ( _proggouraud.uniloc[8], 1, &sh );
	  glUniform1ui(_proggouraud.uniloc[9], _texid);

   glBindVertexArray ( va[0] );
   glBindTexture(GL_TEXTURE_2D, _texid);
   glDrawArrays ( GL_TRIANGLES, 0, _numpoints );
   glBindVertexArray(0); // break the existing vertex array object binding.
 }

