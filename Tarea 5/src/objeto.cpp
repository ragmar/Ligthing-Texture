#pragma once
#include "objeto.h"
#include <iostream>
using namespace std;


		objeto::objeto(){
				t=0.02f;
				estado=true;
		}

		objeto::~objeto(){
			individual_box.clear();
			obj.clear();
			vertex.clear();
			cara_vertice.clear();
			cara_normal.clear();
			cara_textura.clear();
			texturas.clear();
			origen.clear();
			norma_point.clear();
		}

		void objeto::agregarVertice(point vertice){
			vertex.push_back(vertice);
			if(vertex.size()==1){
				bounding_box.inicializar(vertice);
			}else{
				bounding_box.asignar_menor(vertice);
				bounding_box.asignar_mayor(vertice);
			}
		}

		void objeto::agregarCara(int *C,int *T,int *N){
			cara Aux;
			Aux.V[0]=C[0];
			Aux.V[1]=C[1];
			Aux.V[2]=C[2];
			cara_vertice.push_back(Aux);
			if(T[0]!=-1){
				Aux.V[0]=T[0];
				Aux.V[1]=T[1];
				Aux.V[2]=T[2];
				cara_textura.push_back(Aux);
			}

			if(N[0]!=-1){
				Aux.V[0]=N[0];
				Aux.V[1]=N[1];
				Aux.V[2]=N[2];
				cara_normal.push_back(Aux);
			}
		}
		
		void objeto::agregarNormal(point punto){
			norma_point.push_back(punto);
			
		}

		void objeto::terminoObjeto()
		{
			unsigned int inicial=0;
			obj.push_back(cara_vertice.size());
			if(obj.size()>1)
			{
				inicial=obj[obj.size()-2];
			}
			box auxiliar;
			for(unsigned int i=inicial;i<cara_vertice.size();i++)
			{
				if(i==inicial)
				{
					auxiliar.inicializar(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[2]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[2]]);
				}else
				{
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[0]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[1]]);
					auxiliar.asignar_menor(vertex[cara_vertice[i].V[2]]);
					auxiliar.asignar_mayor(vertex[cara_vertice[i].V[2]]);
				}
			
			}
			individual_box.push_back(auxiliar);
		}

		void objeto::agregarTextura(float x, float y){
			text pos_text;
			pos_text.T[0]=x;
			pos_text.T[1]=y;
			texturas.push_back(pos_text);
		}

		void objeto::normalizar(){ //poner todo entre 0 y 1
			float norm=bounding_box.normalizar();

			bounding_box.max=bounding_box.max/norm;
			bounding_box.min=bounding_box.min/norm;
			for(unsigned int i=0;i<vertex.size();i++){
				vertex[i]=vertex[i]/norm;
			}
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].max=individual_box[i].max/norm;
				individual_box[i].min=individual_box[i].min/norm;
			}
		}

		void objeto::sett(float t){
			this->t=t;
		}

		void objeto::CalcularNorma(){//sacar la norma
			if(cara_normal.size()==0){
				float unitario;
				origen.clear();
				norma_point.clear();
				cara_normal.clear();

				origen.resize(cara_vertice.size());
				cara_normal.resize(cara_vertice.size());
				norma_point.resize(vertex.size());
				
				for(unsigned int i=0;i<norma_point.size();i++){
					norma_point[i].x=0.0f;
					norma_point[i].y=0.0f;
					norma_point[i].z=0.0f;
				}

				point u,v,norma;
				for(unsigned int i=0;i<cara_vertice.size();i++){
					cara_normal[i]=cara_vertice[i];
					u=vertex[cara_vertice[i].V[1]]-vertex[cara_vertice[i].V[0]];
					v=vertex[cara_vertice[i].V[2]]-vertex[cara_vertice[i].V[0]];
					//sacar produto cruz
					norma=productoCruz(u,v);
					unitario=norma.norma2();
					norma=norma/unitario; 

					origen[i]=(vertex[cara_vertice[i].V[0]]+vertex[cara_vertice[i].V[1]]+vertex[cara_vertice[i].V[2]])/3.0f;

					norma_point[cara_vertice[i].V[0]]=norma_point[cara_vertice[i].V[0]]+norma;
					norma_point[cara_vertice[i].V[1]]=norma_point[cara_vertice[i].V[1]]+norma;
					norma_point[cara_vertice[i].V[2]]=norma_point[cara_vertice[i].V[2]]+norma;
				}
				for(unsigned int i=0;i<norma_point.size();i++){
					unitario=norma_point[i].norma2();
					norma_point[i]=norma_point[i]/unitario; 
				}
			}
		}

		point objeto::productoCruz(point u, point v){
			point A;
			A.x=u.y*v.z-u.z*v.y;
			A.y=u.z*v.x-u.x*v.z;
			A.z=u.x*v.y-u.y*v.x;
			return A;
		}

		void objeto::centrar(){
			point PM;
			PM=bounding_box.Medio();
			bounding_box.max=bounding_box.max-PM;
			bounding_box.min=bounding_box.min-PM;
			for(unsigned int i=0;i<vertex.size();i++){
				vertex[i]=vertex[i]-PM;
			}
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].max=individual_box[i].max-PM;
				individual_box[i].min=individual_box[i].min-PM;
			}
		}

		void objeto::Pintar(){
			for(unsigned int i=0;i<cara_vertice.size();i++){
				glBegin(GL_TRIANGLES);
					
					glTexCoord2f(texturas[cara_textura[i].V[0]].T[0], texturas[cara_textura[i].V[0]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[0]].x,norma_point[cara_normal[i].V[0]].y,norma_point[cara_normal[i].V[0]].z);
					vertex[cara_vertice[i].V[0]].Pintar();
					
					glTexCoord2f(texturas[cara_textura[i].V[1]].T[0], texturas[cara_textura[i].V[1]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[1]].x,norma_point[cara_normal[i].V[1]].y,norma_point[cara_normal[i].V[1]].z);
					vertex[cara_vertice[i].V[1]].Pintar();
					
					glTexCoord2f(texturas[cara_textura[i].V[2]].T[0], texturas[cara_textura[i].V[2]].T[1]);
					glNormal3f(norma_point[cara_normal[i].V[2]].x,norma_point[cara_normal[i].V[2]].y,norma_point[cara_normal[i].V[2]].z);
					vertex[cara_vertice[i].V[2]].Pintar();

				glEnd();
			}
		}

		void objeto::PintarNormales(bool norm,bool norm_p){
			point D;
			if(norm_p){
				for(unsigned int i=0;i<norma_point.size();i++){
					glBegin(GL_LINES);
						vertex[i].Pintar();
						D=vertex[i]+norma_point[i]*t;
						D.Pintar();
					glEnd();
				}
			}
		}

		void objeto::clear(){
			t=0.02f;
			individual_box.clear();
			obj.clear();
			vertex.clear();
			cara_vertice.clear();
			cara_normal.clear();
			cara_textura.clear();
			texturas.clear();
			origen.clear();
			norma_point.clear();
		}

		void objeto::Pintar_box(){
			for(unsigned int i=0;i<individual_box.size();i++)
			{
				individual_box[i].Pintar();
			}
			//bounding_box.Pintar();
		}

