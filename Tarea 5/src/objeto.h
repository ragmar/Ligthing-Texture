#pragma once
#include "point.h"
#include "box.h"
#include <vector>
using namespace std;

class objeto
{
public:
	objeto();
	~objeto();
	box bounding_box;
	bool estado;
	void agregarVertice(point vertice);
	void agregarCara(int *C,int *T,int *N);
	void normalizar();
	void sett(float t);
	void CalcularNorma();
	point productoCruz(point u, point v);
	void centrar();
	void Pintar();
	void PintarNormales(bool norm,bool norm_p);
	void clear();
	void Pintar_box();
	void agregarNormal(point punto);
	void agregarTextura(float x,float y);
	void terminoObjeto();
	vector <box> individual_box;
	vector <point> vertex;

private:
	struct cara{
		int V[3];
	};
	struct text{
		float T[2];
	};
	vector <int> obj;
	vector <point> origen;
	vector <text> texturas;
	vector <point> norma_point;

	vector <cara> cara_vertice;
	vector <cara> cara_normal;
	vector <cara> cara_textura;
	float t;

};