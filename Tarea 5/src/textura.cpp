#pragma once
#include "textura.h"

	
	textura::textura(){
		activa=true;
	}
	textura::~textura(){
	
	}

	void textura::Init(string fname){
		archivo=fname;
	}


	void textura::CargarTextura(){
		GLint enteroAux;
		FREE_IMAGE_FORMAT fifmt = FreeImage_GetFileType(archivo.c_str(), 0);

		FIBITMAP *dib = FreeImage_Load(fifmt, archivo.c_str(),0);

		dib = FreeImage_ConvertTo24Bits(dib);
		
		if( dib != NULL )
		{
			glGenTextures( 1, &tex );
			glBindTexture(GL_TEXTURE_2D, tex );
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);

			BYTE *bits = new BYTE[FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib) * 3];

			BYTE *pixels = (BYTE*)FreeImage_GetBits(dib);
			enteroAux = FreeImage_GetWidth(dib) * FreeImage_GetHeight(dib);


				for(int pix=0; pix < enteroAux; pix++)
				{
					bits[pix*3+0]=pixels[pix*3+2];
					bits[pix*3+1]=pixels[pix*3+1];
					bits[pix*3+2]=pixels[pix*3+0];

				}
			     
				glTexImage2D( GL_TEXTURE_2D, 0, 3, FreeImage_GetWidth(dib), FreeImage_GetHeight(dib), 0,
						GL_RGB, GL_UNSIGNED_BYTE, bits );

				
				FreeImage_Unload(dib);
				delete bits;
		}
	}

	void textura::Bind(){
		glBindTexture( GL_TEXTURE_2D, tex );
	}
