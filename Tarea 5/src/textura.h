#pragma once
#pragma comment (lib,"lib/freeglut.lib")
#include "../tools/GL/freeglut.h"
#pragma comment (lib,"lib/FreeImage.lib")
#include "../tools/FreeImage/FreeImage.h"
#include <string>

using namespace std;




class textura
{
	private:
		GLuint tex;
		string archivo;
	public:
		bool activa;
		textura();
		~textura();
		void Init(string fname);
		void CargarTextura();
		void Bind();
};