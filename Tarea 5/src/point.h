#pragma once
#pragma comment (lib,"lib/freeglut.lib")
#include "../tools/GL/freeglut.h"
#include <cmath>
using namespace std;


class point{
public:
		float x;
		float y;
		float z;
		point(float x,float y,float z);
		point();
		~point();
		void operator=(point punto);
		point operator*(point punto);
		point operator*(float cantidad);
		point operator/(float cantidad);
		point operator+(point punto);
		point operator-(point punto);
		float norma2();
		point MedioFigura();
		void Pintar();
private:
};