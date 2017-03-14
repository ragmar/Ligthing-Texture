#pragma comment (lib,"lib/freeglut.lib")//incluyo al libreria de free glut
#pragma comment (lib,"lib/AntTweakBar.lib")
#include "../tools/GL/freeglut.h"
#include "../tools/AntTweakBar/AntTweakBar.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "objeto.h"
#include "textura.h"
#include "quaternions.h"
#include "carga_archivo.h"
#include "MapaAltura.h"

#define PI 3.141592f

#define SQRT3 1.7320508f
#define OBJECT 22
#define LIGHT_ACTIVADA 1

using namespace std;




GLint w;
GLint h;
GLfloat xCol;
GLfloat yCol;
GLfloat zCol;
GLfloat xMir;
GLfloat yMir;
GLfloat zMir;
GLuint Escenario;


float X=0.0f,Y=0.0f,Z=0.0f;


textura texture[OBJECT];
objeto figura[OBJECT];
int activo[OBJECT];

bool iluminacion;
int Luz=0,Modelo=0;


/*material*/
float materialAmbiental[] = {0.5f,0.5f,0.5f};
float materialDifuso[] = {0.5f,0.5f,0.5f};
float materialEspecular[] = {0.5f,0.5f,0.5f};
float materialBrillo = 20.0f;

/*luz*/
float luzAmbiental[3];
float luzDifusa[3];
float luzEspecular[3];
float luzDireccion[4];
GLfloat global[4];

MapaAltura Colision;


GLvoid mirar() {
	if(xMir > 45.0)
		xMir = 45.0;
	else if(xMir < -45.0)
		xMir = -45.0;
	while(yMir < 0.0)
		yMir += 360.0;
	while(yMir >= 360.0)
		yMir -= 360.0;
}

GLvoid camara() {
	glLoadIdentity();
	glRotatef(xMir, 1.0, 0.0, 0.0);
	glRotatef(yMir, 0.0, 1.0, 0.0);
	glTranslated(-xCol, -5.0f, -zCol);
}

void  lecturaOBJ(string OBJ,int indice){
	figura[indice].clear();
	ifstream entrada(OBJ.c_str(),ios::in);
	point XYZ;
	string letra,aux_letra;
	string slash ("/");
	size_t found;

	int T,C,N;
	int cara[3];
	int c_normal[3];
	int textura[3];
	int objetos =0;
	if(entrada.is_open()){
		entrada>>letra;
		while(!entrada.eof()){
			if(letra=="v"){
				entrada>>XYZ.x>>XYZ.y>>XYZ.z;
				figura[indice].agregarVertice(XYZ);
			}
			if(letra=="vt"){
				entrada>>XYZ.x>>XYZ.y;
				figura[indice].agregarTextura(XYZ.x,XYZ.y);
			}
			if(letra=="vn"){
				entrada>>XYZ.x>>XYZ.y>>XYZ.z;
				figura[indice].agregarNormal(XYZ);
			}
			if(letra=="o")
			{
				entrada>>letra;
				if(objetos!=0){
					figura[indice].terminoObjeto();
					objetos++;
				}
			}
			if(letra=="f"){
				if(objetos==0) objetos++;
				int i=0;
				while((entrada>>letra)&&(letra[0]>=48&&letra[0]<=57)){
					C=0;T=0;N=0;

					found=letra.find_first_of(slash);
					if (found!=string::npos){
						aux_letra=letra.substr(0,found);
						C=atoi(aux_letra.c_str());

						letra=letra.substr(found+1);

						found=letra.find_first_of(slash);
						if(letra[0]=='/'){
							//vertice y norma
							letra=letra.substr(1);
							N=atoi(letra.c_str());

						}else if (found!=string::npos){
							//vertice textura y norma
							aux_letra=letra.substr(0,found);
							T=atoi(aux_letra.c_str());
							letra=letra.substr(found+1);
							N=atoi(letra.c_str());

						}else{
							//vertice y textura
							aux_letra=letra.substr(found+1,letra.size());
							T=atoi(aux_letra.c_str());
						}
					}else{
						//solo el vertice
						C=atoi(letra.c_str());
					}
					//agregar cara
					if(i==0){
						cara[0]=C-1;
						textura[0]=T-1;
						c_normal[0]=N-1;
						i++;
					}else if(i==1){
						cara[1]=C-1;
						textura[1]=T-1;
						c_normal[1]=N-1;
						i++;
					}else{
						cara[2]=C-1;
						textura[2]=T-1;
						c_normal[2]=N-1;
						figura[indice].agregarCara(cara,textura,c_normal);
						cara[1]=cara[2];
						textura[1]=textura[2];
						c_normal[1]=c_normal[2];
						
					}
				}
				continue;
			}
			entrada>>letra;
		}

	}else{
		cout<<"no se pudo abrir el archivo"<<endl;	
	}
	figura[indice].terminoObjeto();
	//figura[indice].normalizar();
//	figura[indice].centrar();
	figura[indice].CalcularNorma();
	cout<<"s"<<endl;
	for(unsigned int i=0;i<figura[indice].individual_box.size();i++)
	{
		cout<<indice<<endl;
		Colision.AgregarObjeto(figura[indice].individual_box[i].min,figura[indice].individual_box[i].max);
	}

}

void CargarModelo(int Modelo,string archivo){
	if(archivo!=""){
		lecturaOBJ(archivo, Modelo);
	}
}

void FilesToLoad(int Modelo){
	string archivo="";
	string archivo_textura="";
	if(Modelo==0){
		archivo="./files/Modelos/abajo.obj";
		archivo_textura="./files/Texturas/Skybox/Down.jpg";
	}else if(Modelo==1){
		archivo="./files/Modelos/adelante.obj";
		archivo_textura="./files/Texturas/Skybox/Front.jpg";
	}else if(Modelo==2){
		archivo="./files/Modelos/atras.obj";
		archivo_textura="./files/Texturas/Skybox/Back.jpg";
	}else if(Modelo==3){
		archivo="./files/Modelos/derecha.obj";
		archivo_textura="./files/Texturas/Skybox/Right.jpg";
	}else if(Modelo==4){
		archivo="./files/Modelos/izquierda.obj";
		archivo_textura="./files/Texturas/Skybox/Left.jpg";
	}else if(Modelo==5){
		archivo="./files/Modelos/arriba.obj";
		archivo_textura="./files/Texturas/Skybox/Up.jpg";
	}else if(Modelo==6){
		archivo="./files/Modelos/techo.obj";
		archivo_textura="./files/Texturas/techo.jpg";
	}else if(Modelo==7){
		archivo="./files/Modelos/truck.obj";
		archivo_textura="./files/Texturas/watertruck.bmp";
	}else if(Modelo==8){
		archivo="./files/Modelos/columnas.obj";
		archivo_textura="./files/Texturas/verde.jpg";
	}else if(Modelo==9){
		archivo="./files/Modelos/borde.obj";
		archivo_textura="./files/Texturas/borde.jpg";
	}else if(Modelo==10){
		archivo="./files/Modelos/jeep.obj";
		archivo_textura="./files/Texturas/jeep.jpg";
	}else if(Modelo==11){
		archivo="./files/Modelos/thor.obj";
		archivo_textura="./files/Texturas/thor.jpg";
	}else if(Modelo==12){
		archivo="./files/Modelos/ironman.obj";
		archivo_textura="./files/Texturas/ironman.jpg";
	}else if(Modelo==13){
		archivo="./files/Modelos/oiltank.obj";
		archivo_textura="./files/Texturas/oiltank.png";
	}else if(Modelo==14){
		archivo="./files/Modelos/basura.obj";
		archivo_textura="./files/Texturas/basura.jpg";
	}else if(Modelo==15){
		archivo="./files/Modelos/barrel.obj";
		archivo_textura="./files/Texturas/barrel.png";
	}else if(Modelo==16){
		archivo="./files/Modelos/cow.obj";
		archivo_textura="./files/Texturas/cow.jpg";
	}else if(Modelo==17){
		archivo="./files/Modelos/bench.obj";
		archivo_textura="./files/Texturas/bench.jpg";
	}else if(Modelo==18){
		archivo="./files/Modelos/r2d2.obj";
		archivo_textura="./files/Texturas/r2d2.tga";
	}else if(Modelo==19){
		archivo="./files/Modelos/box.obj";
		archivo_textura="./files/Texturas/box.jpg";
	}else if(Modelo==20){
		archivo="./files/Modelos/tabla.obj";
		archivo_textura="./files/Texturas/box.jpg";
	}else if(Modelo==21){
		archivo="./files/Modelos/Wheatley.obj";
		archivo_textura="./files/Texturas/Wheatley.jpg";
	}
	CargarModelo(Modelo,archivo);
	texture[Modelo].Init(archivo_textura);
	texture[Modelo].CargarTextura();
}

void TW_CALL CargarTextura(void *clientData){
	int *x=(int*)clientData;

	string archivo=openfilenameImg();
	if(archivo!=""){
		texture[*x].Init(archivo);
		texture[*x].CargarTextura();
	}
}


void TW_CALL CopyStdStringToClient(string& destinationClientString, const string& sourceLibraryString)
{
  // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
  destinationClientString = sourceLibraryString;
}

void TW_CALL pressExit(void *clientData)
{ 
	TwTerminate();
	exit(0);
}


void init(){

	Escenario = glGenLists(OBJECT);


	iluminacion = true;
	Colision.limpiar();
	for(int i=0;i<OBJECT;i++){
		activo[i]=i;
		FilesToLoad(i);
		glNewList(Escenario+i, GL_COMPILE);
			figura[i].Pintar();
		glEndList();
	}
	glEnable (GL_TEXTURE_2D);

	glEnable(GL_DEPTH_TEST);//zbuffer
	glEnable(GL_CULL_FACE);//BACK FACE
	glEnable(GL_NORMALIZE);
	glDepthFunc(GL_LEQUAL);

	TwInit(TW_OPENGL, NULL);
	TwWindowSize(w,h);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	TwBar *myBar;
	myBar = TwNewBar("Opciones");
	


	TwAddSeparator(myBar,"Transformacion/Iluminacion",NULL);

	TwAddVarRW(myBar, "Iluminar", TW_TYPE_BOOLCPP, &iluminacion, "label='Iluminacion' group=Luces");
	TwAddVarRW(myBar,"Shiny",TW_TYPE_FLOAT,&materialBrillo, "label=Brillo group=Luces min=0 max=128");



	TwAddVarRW(myBar, "ColAmb0",TW_TYPE_COLOR3F,&luzAmbiental,"label=Ambiental group='Luz 0'");
	TwAddVarRW(myBar, "ColEsp0",TW_TYPE_COLOR3F,&luzEspecular,"label=Especular group='Luz 0'");
	TwAddVarRW(myBar, "ColDif0",TW_TYPE_COLOR3F,&luzDifusa,"label=Difusa group='Luz 0'");
	TwAddVarRW(myBar, "LightDir0", TW_TYPE_DIR3F, &luzDireccion[0], " label='Direccion Luz 0'  group='Luz 0'");

	TwAddSeparator(myBar,"Edicion/Archivo",NULL);
	TwCopyStdStringToClientFunc(CopyStdStringToClient);
	TwAddButton(myBar,"Salir", pressExit,NULL,"label='Salir' group=Archivo");
	
	TwDefine("Opciones size='200 500' \n"
			 "Opciones valueswidth=fit \n"
			 "Opciones/'Luz 0' visible=true group=Luces \n"
			 );

}

void Init_Light(){

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHT0);

	luzDireccion[0] = 8000.0f, luzDireccion[1] = -3000.0f,luzDireccion[2] = 4500.0f,luzDireccion[3] = 0.0f;
	luzAmbiental[0] =1.0f,luzAmbiental[1] =1.0f,luzAmbiental[2] =1.0f;
	luzDifusa[0] = 1.0f, luzDifusa[1] = 0.5f,luzDifusa[2] = 0.0f;
	luzEspecular[0] = 1.0f, luzEspecular[1] = 0.5f,luzEspecular[2] = 0.0f;
	global[0] = 1.0f, global[1] = 0.5f,global[2] = 0.0f,global[3]= 0.0f;

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global);
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiental);    
	glLightfv(GL_LIGHT0,GL_DIFFUSE, luzDifusa); 
	glLightfv(GL_LIGHT0,GL_SPECULAR, luzEspecular);  

	glMaterialfv(GL_FRONT,GL_AMBIENT, materialAmbiental);
	glMaterialfv(GL_FRONT,GL_DIFFUSE, materialDifuso);
	glMaterialfv(GL_FRONT,GL_SPECULAR, materialEspecular);
	glMaterialf(GL_FRONT,GL_SHININESS, materialBrillo);




}

void LucesDir(){
	glLightfv(GL_LIGHT0,GL_DIFFUSE, luzDifusa); 
	glPushMatrix();
		//float Matriz[16];
		//Quaternion aux;
		//aux.CreateFromAxisAngle(0.0f,1.0f,0.0f,-rotl*3.0f);
		//aux.CreateMatrix(Matriz);
		//glMultMatrixf(Matriz);
		glTranslatef(-luzDireccion[0],-luzDireccion[1],-luzDireccion[2]);	
		glLightfv(GL_LIGHT0, GL_POSITION, luzDireccion);
		gluSphere(gluNewQuadric(), 100, 10, 10);
		glEnable(GL_LIGHTING);

	glPopMatrix();


}

void movimiento(int x,int y){
	if(!TwEventMouseMotionGLUT(x,y) ){


	}

	glutPostRedisplay();

}

void Draw(){
	float Rotacion[16]; //matriz que se pasara a OpenGL
	float v[4];
	v[3]=1.0f;
    glPushMatrix();//salvo la traslacion y el escalamiento

	ConvertQuaternionToMatrix(g_Rotation, Rotacion);//convierto mi quaternion 2*2 en una matriz 4*4 para que OpenGL la entienda
    glMultMatrixf(Rotacion);//multiplico la matriz de OpenGL por la matriz 4*4



	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);//rellena la figura

	for(int i=0;i<OBJECT;i++){
		if(texture[i].activa){
			texture[i].Bind();
		}else{
			glBindTexture( GL_TEXTURE_2D, 0 );
		}
		if(figura[i].estado)
			glCallList(Escenario+i);
		
	}
	glPopMatrix();
	glLoadIdentity();
}

void teclado(unsigned char key, int x, int y){
	if(	!TwEventKeyboardGLUT(key,x,y)){
		float auxX = xCol, auxZ = zCol;
		if(key == 'w') {
			auxX += GLfloat(sin(yMir / 180 * PI));
			auxZ -= GLfloat(cos(yMir / 180 * PI));
			if(Colision.Colision(auxX,auxZ))//al frente
			{
				xCol=auxX;
				zCol=auxZ;
			}else if(Colision.Colision(xCol,auxZ))//eje z
				zCol=auxZ;
			
			else if(Colision.Colision(auxX,zCol))//eje <
				xCol=auxX;
			
			yCol -= GLfloat(sin(xMir / 180 * PI));	
		}
		if(key == 'd') {
			yMir +=  2.5;
			if(yMir >360)
	
			yMir -= 360;
		}
		if(key == 's') {
			auxX -= GLfloat(sin(yMir / 180 * PI));
			auxZ += GLfloat(cos(yMir / 180 * PI));
			if(Colision.Colision(auxX,auxZ))//al frente
			{
				xCol=auxX;
				zCol=auxZ;
			}else if(Colision.Colision(xCol,auxZ))//eje z
				zCol=auxZ;
			else if(Colision.Colision(auxX,zCol))//eje x
				xCol=auxX;
			yCol += GLfloat(sin(xMir / 180 * PI));	
		}
		if(key == 'a') {
			yMir -=  2.5;
			if(yMir < -360)
				yMir += 360;
		}
		if(key == 27)
			exit(0);
	}
	glutPostRedisplay();
}

void teclado_especiales(int key, int x, int y){
	if(	!TwEventSpecialGLUT(key,x,y)){
		if(key == GLUT_KEY_UP) {
			xMir -= 2.5;
			mirar();
		}
		if(key == GLUT_KEY_DOWN) {
			xMir += 2.5;
			mirar();
		}
	}
	glutPostRedisplay();
}

GLvoid idle() {
	glutPostRedisplay();
}

void renderScene(void) {//renderizar

	glClearColor(0.0, 0.0, 0.0, 0.0);	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glEnable(GL_NORMALIZE);
	camara();
	LucesDir();
	Draw();

	TwDraw();
	glutSwapBuffers();
}


void click_izquierdo(int boton, int estado, int x, int y) 
{
	float x1 =2.0f*(float)x/(float)w-1.0f;//redimenciono los puntos al tamaño de mi ventana
	float y1= - (2.0f*(float)y/h - 1.0f);
	if( !TwEventMouseButtonGLUT(boton,estado,x,y) )  // send event to AntTweakBar
	{
		if(estado==GLUT_DOWN){
			X=(float)x;
			Y=(float)y;
			Z=(float)y;
		}
	}
	glutPostRedisplay();
}

void reshape(int x, int y){//redimensiono la pantalla
	
	w = x;
	h = y;
	glViewport(0, 0, w, h);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, ((GLfloat) w) / ((GLfloat) h), 1.0f, 500000.0f);
	glMatrixMode(GL_MODELVIEW);
	TwWindowSize(w, h);
	glLoadIdentity();
}



//Main Principal
int main(int argc, char *argv[])
{	
	w = 800;
	h = 600;
	xCol = yCol = zCol = xMir = yMir = zMir = 0.0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(w,h);
	glutCreateWindow("I.C.G. - Tarea 5 - Alejandro Sans y Victor Felipe");


	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(teclado);//para usar el teclado con AntTweakBar fluidamente
	glutSpecialFunc(teclado_especiales);
	glutMouseFunc(click_izquierdo);
	glutMotionFunc(movimiento);//para mover el mouse con AntTweakBar fluidamente
	init();
	Init_Light();

	glutMainLoop();
	return 0;
}
