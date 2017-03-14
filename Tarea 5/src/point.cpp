#pragma once
#include "point.h"

		point::point(float x,float y,float z){
			this->x=x;
			this->y=y;
			this->z=z;

		}
		point::point(){
			x=0;
			y=0;
			z=0;
		}
		point::~point(){
		}
		void point::operator=(point punto){//igualo puntos
			x=punto.x;
			y=punto.y;
			z=punto.z;
		}

		point point::operator*(point punto){
			point A;
			A.x=x*punto.x;
			A.y=y*punto.y;
			A.z=z*punto.z;
			return A;
		}

		point point::operator*(float cantidad){
			point A;
			A.x=x*cantidad;
			A.y=y*cantidad;
			A.z=z*cantidad;
			return A;
		}

		point point::operator/(float cantidad){
			point A;
			A.x=x/cantidad;
			A.y=y/cantidad;
			A.z=z/cantidad;
			return A;
		}

		point point::operator+(point punto){
			point A;
			A.x=x+punto.x;
			A.y=y+punto.y;
			A.z=z+punto.z;
			return A;
		}
		point point::operator-(point punto){
			point A;
			A.x=x-punto.x;
			A.y=y-punto.y;
			A.z=z-punto.z;
			return A;
		}

		float point::norma2(){
			return sqrt(x*x+y*y+z*z);
		
		}


		point point::MedioFigura(){
			point A(x,y,z);
			return A;
		}

		void point::Pintar(){
			glVertex3f(x,y,z);
		}
