/*=======================================================================
   Copyright 2013 Marcelo Kallmann. All Rights Reserved.
   This software is distributed for noncommercial use only, without
   any warranties, and provided that all copies contain the full copyright
   notice licence.txt located at the base folder of the distribution. 
  =======================================================================*/

# ifndef GS_IMAGE_H
# define GS_IMAGE_H

/** \file gs_image.h 
 * A 24 bit image.*/

# include <gsim/gs_color.h>
# include <gsim/gs_shared.h>

//=========================== GsImage ============================

/*! \class GsImage gs_image.h
    \brief Non compressed 32 bit depth rgba image
    GsImage stores pixel data as a sequence of rgba pixels. */
class GsImage : public GsShared
 { private :
    int _w, _h;    // total image size is 4*_w*_h bytes
    GsColor* _img; // array of rgba values

   public :

    /*! Constructs an empty image */
    GsImage ();

    /*! Destructor */
   ~GsImage ();

    /*! Alloc the desired size in pixels of the image.
        A total of w*h pixels (ie w*h*4 bytes) are allocated.
        Invalid dimensions deletes the image data */
    void init ( int w, int h );

    /*! Changes the image by appying a vertical mirroring */
    void vertical_mirror ();

    /*! Returns the width in pixels of the image */
    int w () const { return _w; }

    /*! Returns the height in pixels of the image */
    int h () const { return _h; }
 
    /*! Returns the number of bytes in the image, equal to w()*h() */
    int size () const { return _w*_h; }

    /*! Returns a reference to the color of pixel at position (l,c) */
    GsColor& operator() ( int l, int c ) { return pixel(l,c); }

    /*! Returns a reference to the color of pixel at position (l,c) */
    GsColor& pixel ( int l, int c ) { return _img[l*_w+c]; }

    /*! Returns a const reference to the color of pixel at position (l,c) */
    const GsColor& cpixel ( int l, int c ) const { return _img[l*_w+c]; }

    /*! Returns a pointer to the pixel color (4 bytes) at position (l,c) */
    GsColor* ptpixel ( int l, int c ) { return &_img[l*_w+c]; }

    /*! Access a pixel in planar xy coordinates (the opposite of line,column) */
    GsColor& xypixel ( int x, int y ) { return _img[y*_w+x]; }

    /*! Const access to a pixel in planar xy coordinates (the opposite of line,column) */
    const GsColor& cxypixel ( int x, int y ) const { return _img[y*_w+x]; }

    /*! Returns the buffer index of the pixel */
    int index ( int l, int c ) const { return l*_w+c; }

    /*! Returns the buffer index in planar xy coordinates */
    int xyindex ( int x, int y ) const { return y*_w+x; }

    /*! Returns a reference to the red component of the pixel at position (l,c) */
    gsbyte& r ( int l, int c ) { return pixel(l,c).r; }

    /*! Returns a reference to the green component of the pixel at position (l,c) */
    gsbyte& g ( int l, int c ) { return pixel(l,c).g; }

    /*! Returns a reference to the blue component of the pixel at position (l,c) */
    gsbyte& b ( int l, int c ) { return pixel(l,c).b; }
 
    /*! Returns a reference to the alpha component of the pixel at position (l,c) */
    gsbyte& a ( int l, int c ) { return pixel(l,c).a; }

    /*! Returns the base pointer of the line l of the image */
    GsColor* line ( int l ) { return &_img[l*_w]; }

    /*! Returns the base pointer of the image data */
    GsColor* data () { return _img; }

    /*! Returns the base pointer of the image data as a const pointer */
    GsColor* cdata () const { return _img; }

    /*! Saves the image in a bmp file. Returns true if could write file or false otherwise. */
    bool save ( const char* filename );

    /*! Load a bmp image. Returns true if could load or false otherwise. */
    bool load ( const char* filename );
 };

//============================= end of file ==========================

# endif // GS_IMAGE_H
