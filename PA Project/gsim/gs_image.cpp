/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# include <gsim/gs_image.h>

//# define GS_USE_TRACE1  // constructor / desctructor
//# define GS_USE_TRACE2  // load 
//# include <gkim/gs_trace.h>

# define STB_IMAGE_IMPLEMENTATION
# define STBI_NO_PSD
# define STBI_NO_TGA
# define STBI_NO_HDR
# define STBI_NO_PIC
# define STBI_NO_GIF
# define STBI_NO_PNM
# define STBI_NO_BMP
# define STBI_NO_HDR
# include "stb_image.h"

# define STB_IMAGE_WRITE_IMPLEMENTATION
# define STB_IMAGE_WRITE_ONLY_PNG // custom define added
# include "stb_image_write.h"

//=========================== GsImage ============================

GsImage::GsImage ()
 {
   //GS_TRACE1 ("Constructor");
   _w = _h = 0;
   _img = 0;
 }

GsImage::~GsImage ()
 {
   //GS_TRACE1 ("Destructor");
   if (_img) stbi_image_free ( (void*)_img );
 }

void GsImage::init ( int w, int h )
 {
   if ( w==_w && h==_h ) return;

   if ( _img ) stbi_image_free ( (void*)_img );

   if ( w<=0 || h<=0 )
    { _w = _h = 0; 
      _img = 0;
    }
   else
    { _w = w; 
      _h = h;
      _img = (GsColor*)stbi__malloc(w*h*sizeof(GsColor));
    }
 }

void GsImage::vertical_mirror ()
 {
   int i, ie, mid;
   mid = _h/2;
 
   GsColor* buf = new GsColor[_w];
   int lsize = _w*sizeof(GsColor);

   for ( i=0,ie=_h-1; i<mid; i++,ie-- )
    { memcpy ( buf,      line(i),  lsize );
      memcpy ( line(i),  line(ie), lsize );
      memcpy ( line(ie), buf,      lsize );
    }
   delete [] buf;
 }

# define PutInt(i)   fwrite(&i,4/*bytes*/,1/*num items*/,f)
# define PutShort(s) fwrite(&s,2/*bytes*/,1/*num items*/,f)

bool GsImage::save ( const char* filename )
 {
   if ( !filename ) return false;
   if ( !filename[0] ) return false;
   if ( filename[strlen(filename)-1]=='g' ) //save in png format
    { return stbi_write_png ( filename, _w, _h, 4, (void*)_img, 0 )==1; }

   FILE* f = fopen ( filename, "wb" );
   if ( !f ) return false;

   int i = 0;
   int offset = 14+40;
   //int dw = 4-(_w%4); if ( dw==4 ) dw=0;
   int dw = (_w%4);

   int filesize = 14 /*header*/ + 40 /*info*/ + (_w*_h*3) +(_h*dw);

   // 14 bytes of header:
   fprintf ( f, "BM" ); // 2 bytes : signature
   PutInt ( filesize ); // file size
   PutInt ( i );        // reserved (zeros)
   PutInt ( offset );   // offset to the data

   // 40 bytes of info header:
   int infosize = 40;
   short planes = 1;
   short bits = 24;
   int compression = 0; // no compression
   int compsize = 0;    // no compression
   int hres = 600;
   int vres = 600;
   int colors = 0;
   int impcolors = 0;   // important colors: all
   PutInt ( infosize ); // size of info header
   PutInt ( _w );       // width
   PutInt ( _h );       // height
   PutShort ( planes );
   PutShort ( bits );
   PutInt ( compression );
   PutInt ( compsize );
   PutInt ( hres );
   PutInt ( vres );
   PutInt ( colors );
   PutInt ( impcolors );

   int w, h;
   GsColor* pixel;
   for ( h=_h-1; h>=0; h-- )
    { pixel = line(h);
      for ( w=0; w<_w; w++ )
       { fputc ( pixel->b, f ); // B
         fputc ( pixel->g, f ); // G
         fputc ( pixel->r, f ); // R
         pixel++;
       }
      for ( w=0; w<dw; w++ ) fputc ( 0, f );
    }

   fclose ( f );
   return true;
 }

bool GsImage::load ( const char* filename )
 {
   //GS_TRACE2 ( "Loading "<<filename<<":" );

   init(0,0);

   int w, h, d; // d = number of 8-bit components per pixel
   _img = (GsColor*) stbi_load ( filename, &w, &h, &d, 4 ); // force 4 components per pixel

   //GS_TRACE2 ( "depth:" << d << ", size:" << w << "x" << h << ", bitsperpixel:"<<d );

   if ( !_img ) { /*GS_TRACE2("Error!");*/ _w=_h=0; return false; }
   _w=w; _h=h;

   //GS_TRACE2 ( "Loaded." );

   return true;
 }

