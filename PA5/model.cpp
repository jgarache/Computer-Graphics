
# include <stdio.h>
# include <string.h>
# include <stdlib.h>

# include "model.h"

#ifdef WIN32
#pragma warning(disable : 4996) // to remove fopen warning as being unsafe
#endif

void Model::load ( const char* file )
 {
   FILE* f = fopen ( file, "rt" );
   if ( !f ) // one more try: try to open again in the ".." folder
    { char s[120];
      sprintf(s,"../%s",file);
      f = fopen ( s, "rt" );
    }
   if ( !f ) // not found
    { printf("Could not load [%s]!\n",file); exit(1); }

   int i;
   char token[256];

   while ( true )
    {
      fscanf ( f, "%s", token );
      if ( feof(f) ) break;

      if ( strcmp(token,"vertices")==0 )
       { fscanf ( f, "%d", &vsize );
         delete[] V;
         V = new GsVec[vsize];
         for ( i=0; i<vsize; i++ )
          { fscanf ( f, "%f %f %f", &V[i].x, &V[i].y, &V[i].z );
          }
       }
      else if ( strcmp(token,"vertices_per_face")==0 )
       { fscanf ( f, "%d", &fsize );
         delete[] F;
         F = new Fac[fsize];
         for ( i=0; i<fsize; i++ )
          { fscanf ( f, "%d %d %d", &F[i].va, &F[i].vb, &F[i].vc );
          }
       }
      else if ( strcmp(token,"normals")==0 )
       { fscanf ( f, "%d", &nsize );
         delete[] N;
         N = new GsVec[nsize];
         for ( i=0; i<nsize; i++ )
          { fscanf ( f, "%f %f %f", &N[i].x, &N[i].y, &N[i].z );
          }
       }
      else if ( strcmp(token,"normals_per_face")==0 )
       { if ( !F ) { printf("Error in file!\n"); exit(1); }
         for ( i=0; i<fsize; i++ )
          { fscanf ( f, "%d %d %d", &F[i].na, &F[i].nb, &F[i].nc );
          }
       }
      else if ( strcmp(token,"colors")==0 )
       { if ( !F ) { printf("Error in file!\n"); exit(1); }
         int a, b, c;
         for ( i=0; i<fsize; i++ )
          { fscanf ( f, "%d %d %d", &a, &b, &c );
            F[i].r = (unsigned char)a;
            F[i].g = (unsigned char)b;
            F[i].b = (unsigned char)c;
          }
       }
    }
 }


