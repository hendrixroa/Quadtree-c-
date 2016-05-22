#include <iostream> 
#include <stdlib.h> 
#include <stdio.h> 
#include <stdio.h> 
#include "FreeImage.h" 
#include <fstream>
#include <string.h>
using namespace std; 
unsigned char* loadImage(const char * imgName, int &width, int &height) 
{ 
 //Se obtiene el formato de la imagen. 
 FREE_IMAGE_FORMAT formatImage = FreeImage_GetFileType(imgName); 
 
 //Se carga la imagen utilizando FreeImage. 
 FIBITMAP* FIbitmap = FreeImage_Load(formatImage, imgName, 0); 
 FIbitmap = FreeImage_ConvertToStandardType(FIbitmap); 
 
 //Se obtiene el ancho y alto de la imagen. 
 width = FreeImage_GetWidth(FIbitmap); 
 height = FreeImage_GetHeight(FIbitmap); 
 
 //Se crea la matriz de colores. 
 unsigned char *b = new unsigned char[width * height]; 
 
 //Se copian los colores a la matriz creada. 
 FreeImage_ConvertToRawBits(b, FIbitmap, width, 8, FI_RGBA_RED_MASK, 0, 0, TRUE); 
 
 //Se cierra el bitmap. 
 FreeImage_Unload(FIbitmap); 
 
 return b; 
} 
char archivo[100];
//clase quadtree donde estaran definidas e implementadas todos lo metodos del arbol a crear.
class QuadTree{
	public:
	char color;    //variable info del nodo
	QuadTree *Cuad0;//apuntador al cuadrante 0
	QuadTree *Cuad1;//apuntador al cuadrante 1
	QuadTree *Cuad2;//apuntador al cuadrante 2
	QuadTree *Cuad3;//apuntador al cuadrante 3
    QuadTree(char colour){//Constructor de la clase quadtree quien en primera instacia tendra el dato y sus hijos a null
		color=colour;
		Cuad0=NULL;
		Cuad1=NULL;
		Cuad2=NULL;
		Cuad3=NULL;
	}
	//Accion que muestra la matriz
	void MostrarMatriz(char Matriz[][10000],int ancho1,int largo1,int in,int in2){
	for(int i2=in;i2<ancho1;i2++){ 
    for(int j2=in2;j2<largo1;j2++){ //muestra todo el conjunto de una fila , y luego pasa a la  siguiente fila
    cout<<Matriz[i2][j2]; 
          }
    cout<<endl;
       }
     } 
	//Funcion que verifica si la matriz es homogenea,en caso de que lo sea retorna true y el elemento del cual es homogenea por ref , sino false
	bool Color(char Matriz[][10000],int ancho1,int largo1,int in,int in2,char& col){
	char elem=Matriz[in][in2];//accedo al elemento inicial para compararlos con los demas 
	if(in==1 || in2==1){ //caso especial donde los limites son 1, pues solo tendrian un valor y desde luego es homogenea.
		col=Matriz[in-1][in2-1]; //se le pasa el valor 
		return true;//y se retorna que es homogenea
	}else{
		for(int i=in;i<ancho1;i++){ //sino..., recorro toda la matriz por fila y verifico que todos sus elementos sean iguales
			for(int j=in2;j<largo1;j++){
				if(elem != Matriz[i][j]){ //basta con haber solo 1 diferente para decir que no es homogenea
					return false; 
				} 
			}
		}
	col=elem;
	return true;
	}
}
	//Accion que crea el arbol 
	 void CrearArbol(char Matriz[][10000],int ancho,int alto,int i ,int j,char& c,QuadTree *&MyTree){
	if(i > ancho || j> alto) return;//Verifico que los limites se encuentren correctos 
	if(Color(Matriz,ancho,alto,i,j,c)){// verificacion para saber si es homogenea 
		if(c=='1'){ //si es homogenea y ademas todos sus caracteres son 1 
			MyTree = new QuadTree('1');	//creo un nodo blanco
		}
	 else if(c=='0'){//si es homogenea pero ademas el caracter es 0
			MyTree = new QuadTree('0'); //creo un nodo negro
		}
	 }else{//pero si no es homogenea 
        MyTree = new QuadTree('2');//creo un nodo gris, que en este caso sera un nodo padre. 
        //y divido la matriz en 4 cuadrantes, con 4 llamadas recursivas donde cada una tendra el cuadrante correspondiente
        //indices , y limites respectivamente.
		CrearArbol(Matriz,i+(ancho-i)/2,j+(alto-j)/2,i,j,c,MyTree->Cuad0);
		CrearArbol(Matriz,i+(ancho-i)/2,j+(alto-j),i,j+(alto-j)/2+1,c,MyTree->Cuad1);
		CrearArbol(Matriz,i+(ancho-i),j+(alto-j),i+(ancho-i)/2+1,j+(alto-j)/2+1,c,MyTree->Cuad2);
		CrearArbol(Matriz,i+(ancho-i),j+(alto-j)/2,i+(ancho-i)/2+1,j,c,MyTree->Cuad3);
	 }}
        //Accion que muestra una vez creado, la estructura del arbol en memoria con recorrido preorden. 
        void RecorridoPreorden(QuadTree *MyTree){
	    if(MyTree!=NULL){
		cout<<MyTree->color;
		RecorridoPreorden(MyTree->Cuad0);
	    RecorridoPreorden(MyTree->Cuad1);
	    RecorridoPreorden(MyTree->Cuad2);
	    RecorridoPreorden(MyTree->Cuad3);
	}}
	//Accion que rota los apuntadores de un arbol una vez creado en memoria 
	void Espejo(QuadTree *MyTree){
    if(MyTree!=NULL){
			QuadTree *Aux,*Aux2;//creo dos apuntadores al arbol
			Aux=MyTree->Cuad0;//guardo en aux el apuntador al primer cuadrante
			MyTree->Cuad0=MyTree->Cuad1;//al cuadrante 0 le asigno el cuadrante 1
			MyTree->Cuad1=Aux;//luego al cuadrante 1, le asigno el cuadrante 0
			Aux2=MyTree->Cuad2;//guardo en aux2 el cuadrante 2
			MyTree->Cuad2=MyTree->Cuad3;//al cuadrante 2 le asigno el cuadrante 3 
			MyTree->Cuad3=Aux2;//y al cuadrante 3 le asigno el 2
		Espejo(MyTree->Cuad0);
	    Espejo(MyTree->Cuad1);
	    Espejo(MyTree->Cuad2);
	    Espejo(MyTree->Cuad3);
	}
	}
	//Accion que una vez que se crea un arbol en memoria, y son rotados los apuntadores, crea una matriz a partir de ellos
	void CrearImagen(char Matriz[][10000],int ancho,int alto,int i ,int j,QuadTree *MyTree){
	if(MyTree->color=='1'){ //verifica si un nodo es blanco, si lo es,llena la matriz con los indices y limites correspondiendes de 1
		for(int i2=i;i2<=ancho;i2++){
			for(int j2=j;j2<=alto;j2++){
				Matriz[i2][j2]='1';	
			}
		}
	}
	else if(MyTree->color=='0'){ //si el nodo es negro , lo llena con 0
		for(int i3=i;i3<=ancho;i3++){
			for(int j3=j;j3<=alto;j3++){
				Matriz[i3][j3]='0';
			}
		}
	}else{// sino es blanco ni negro , entonces es gris, llamo recursivamente 
		//dividiendola en cuadrantes para verficar su color y llenar la matriz.
		CrearImagen(Matriz,i+(ancho-i)/2,j+(alto-j)/2,i,j,MyTree->Cuad0);
		CrearImagen(Matriz,i+(ancho-i)/2,j+(alto-j),i,j+(alto-j)/2+1,MyTree->Cuad1);
		CrearImagen(Matriz,i+(ancho-i),j+(alto-j),i+(ancho-i)/2+1,j+(alto-j)/2+1,MyTree->Cuad2);
		CrearImagen(Matriz,i+(ancho-i),j+(alto-j)/2,i+(ancho-i)/2+1,j,MyTree->Cuad3);
	}
	}
//Accion que cambia los colores de los nodos hojas, si es 1 , lo cambia a 0 , y viceversa	
	void InvertirColor(QuadTree *MyTree){
    if(MyTree!=NULL){
		//voy verificando que cuadrante no es null,verifico su color, dependiendo de que color sea 
		// lo cambio, asi para todos los cuadrantes.  
			 if(MyTree->Cuad0 != NULL && MyTree->Cuad0->color=='1'){
				MyTree->Cuad0->color='0';
			}
			else if(MyTree->Cuad0 != NULL && MyTree->Cuad0->color=='0'){
				MyTree->Cuad0->color='1';
			}
			 if(MyTree->Cuad1 != NULL && MyTree->Cuad1->color=='0'){
				MyTree->Cuad1->color='1';
			}
			else if(MyTree->Cuad1 != NULL && MyTree->Cuad1->color=='1'){
				MyTree->Cuad1->color='0';
			}
			 if(MyTree->Cuad2 != NULL && MyTree->Cuad2->color=='0'){
				MyTree->Cuad2->color='1';
			}
			else if(MyTree->Cuad2 != NULL && MyTree->Cuad2->color=='1'){
				MyTree->Cuad2->color='0';
			}
			 if(MyTree->Cuad3 != NULL && MyTree->Cuad3->color=='1'){
				MyTree->Cuad3->color='0';
			}
			else if(MyTree->Cuad3 != NULL && MyTree->Cuad3->color=='0'){
				MyTree->Cuad3->color='1';
			}
		InvertirColor(MyTree->Cuad0);
	    InvertirColor(MyTree->Cuad1);
	    InvertirColor(MyTree->Cuad2);
	    InvertirColor(MyTree->Cuad3);   
	}
	}
	   //Accion que cuenta el numero de nodos, dependiendo de que color es
	void NumeroColores(QuadTree *MyTree,int& B,int& N,int& G){	
    if(MyTree!=NULL){
			if(MyTree->color=='2'){
				G++;
			}
		   else	if(MyTree->color=='1'){
				B++;
			}
			else if(MyTree->color=='0'){
			  N++;
             }		
		NumeroColores(MyTree->Cuad0,B,N,G);
	    NumeroColores(MyTree->Cuad1,B,N,G);
	    NumeroColores(MyTree->Cuad2,B,N,G);
	    NumeroColores(MyTree->Cuad3,B,N,G);
	}
	}
	  //Funcion que retorna el maximo de 4 variables 
	int Max(int c1,int c2,int c3,int c4){
		int m1,m2;
		if(c1 >= c2){
			m1=c1;
		}
		else{
			m1=c2;
		}
		if(c3 >= c4){
			m2=c3;
		}
		else{
			m2=c4;
		}
		if(m1 >= m2){
			return m1;
		}
		else{
			return m2;
		}
	}
	//Funcion que calcula la altura de un arbol
	int Altura(QuadTree *MyTree){
		if(MyTree!=NULL){
			if(MyTree->color=='0' || MyTree->color=='1'){//si es una hoja, su altura siempre es 0
				return 0;
			}else{//sino llamo Max , con todas las alturas de todos los cuadrantes.
		      return Max(Altura(MyTree->Cuad0),Altura(MyTree->Cuad1),Altura(MyTree->Cuad2),Altura(MyTree->Cuad3))+1;		
			}
		}	
	}
	//Accion que guarda en un string el recorrido preorden de un arbol
	void SalvarImagen(QuadTree *MyTree,string& ruta){
	    if(MyTree!=NULL){//si no es nulo en la variable ruta guardo el acumulado de todos los caracteres
	       ruta = ruta + MyTree->color;
		SalvarImagen(MyTree->Cuad0,ruta);
	    SalvarImagen(MyTree->Cuad1,ruta);
	    SalvarImagen(MyTree->Cuad2,ruta);
	    SalvarImagen(MyTree->Cuad3,ruta);
	   }
	}
	 //Accion que destruye un arbol.
	void EliminarArbol(QuadTree *MyTree){
	    if(MyTree!=NULL){
		EliminarArbol(MyTree->Cuad0);
	    EliminarArbol(MyTree->Cuad1);
	    EliminarArbol(MyTree->Cuad2);
	    EliminarArbol(MyTree->Cuad3);
	    delete MyTree; 
	}}
	//Accion que dado un string donde contiene un string con el recorrido preorden de un arbol,lo arma en memoria
	void ArmarDesdeQt(string linea,int indice,int tam,QuadTree *&MyTree){
		if(indice > tam)return;//si se sobrepasa los limites se retorna
		if(linea[indice]=='1' || linea[indice]=='0'){ //verifico que no sea raiz
		if(linea[indice]=='1'){//si es 1 , hago un nodo blanco
			MyTree=new QuadTree('1');
		}
	   else	if(linea[indice]=='0'){//si es 0, hago un nodo negro
			MyTree=new QuadTree('0');
		}
	}else{//sino es porque es una raiz
		MyTree=new QuadTree('2');
		//llamo recusivamente con los 4 subsiguientes indices del string para verificar si son hojas o padres.
         ArmarDesdeQt(linea,indice+1,tam,MyTree->Cuad0);
         ArmarDesdeQt(linea,indice+2,tam,MyTree->Cuad1);
         ArmarDesdeQt(linea,indice+3,tam,MyTree->Cuad2);
         ArmarDesdeQt(linea,indice+4,tam,MyTree->Cuad3);			
	}
}	
 };
//funcion que valida la extension de un archivo
bool EsValido(string ar,bool& Esqt){
	int tam = ar.length(); //se le saca el tama#o para hacer substring
	string image = ar.substr(tam-4,tam);//aqui hago un substring desde el limite superio hasta -4 para verificar si es .png,.bmp,.jpg
	string arch = ar.substr(tam-3,tam);//aqui es por si es .qt
	if(image==".bmp" || image==".jpg" || image==".png"){//si es cualquiera de las extensiones antes citadas: se retorna true ,y se descarta que sea .qt 
		Esqt=false;
		return true;
	}
	else if(arch==".qt"){//pero si es qt , se retorna true y esqt en true
		Esqt=true;
		return true;
	}
	return false;//si no es ninguno de estos casos se retorna false(extension invalida)
}
//funcion que valida que un numero sea potencia de 2
bool EsPot2(int num){
	int acum=1;//variable acumuladora
	while(true){	
	if(acum==num){//si el acumulado es igual al numero
		return true;//se retorna true
		break;
	}
	if(acum > num){//si se sobrepasa el acumulado es que el numero no es potencia de 2
		return false;
		break;
	}
    acum *= 2;//voy acumulando la multiplicacion de 2
   }
}
//funcion que valida que una ruta tenga formato .qt
bool EsForqt(string rut){
	int tam = rut.length();
	string arch = rut.substr(tam-3,tam);
	if(arch==".qt"){
		return true;
	}
	return false;
}

char Matriz[10000][10000];
int main() 
{ 
int opcion;
QuadTree *MyTree;//variable objeto del arbol 
MyTree=NULL;//Al principio tenemos un arbol vacio
int B=0,N=0,G=0,H=0;//variables para los colores y la altura
unsigned char *imagen; //puntero a char para la carga de la imagen
	int alto,ancho,indice,tam;
	string nomArch,aux,linea;
	bool Esqt,val,homogenea;
	int i1=0,j1=0;//Variables indices
	char c;//variable para el color
	fstream f1;
	
MENU:
do{
cout<<"1. Cargar Imagen(formato .bmp, .jpg, .png, .qt)." <<endl;
cout<<"2. Salvar Imagen en formato quadtree(formato con extension .qt)." <<endl;
cout<<"3. Imprimir el numero de nodos Blancos,Negros,Grises y la altura del arbol." <<endl;
cout<<"4. Invertir los colores."<<endl;
cout<<"5. Imprimir en Pre-orden."<<endl;
cout<<"6. Flip Horzontal(Espejo en X)."<<endl;
cout<<"7. Mostrar Imagen."<<endl;
cout<<"8. Salir."<<endl;
cin>>opcion;
}while(opcion < 1 || opcion > 8);	
	if(opcion==1){	
	cout<<"Ingrese el Nombre del archivo :"<<endl;
    cin>>nomArch;
	val=EsValido(nomArch,Esqt);//validamos la extension
	if(val==true && Esqt==false){//caso en que sea .bmp,.png,.jpg
	imagen=loadImage(nomArch.c_str(),ancho,alto);//accion que carga la imagen
	if(EsPot2(ancho)==true && EsPot2(alto)==true && alto==ancho){
		//Llenado de matriz
	for(int i=0;i<alto;i++){ //una vez cargada la imagen se hace el debido llenado.
	for(int j=0;j<ancho;j++){ 
	if(imagen[i*ancho+j]>0){
	Matriz[i][j]='1'; 
	}else{
	Matriz[i][j]='0';
	}}} 
	  MyTree->CrearArbol(Matriz,ancho,alto,i1,j1,c,MyTree);//llamo a crear arbol
		goto MENU;
	}else{
	cout<<"Error,el ancho y el alto de la imagen no son potencia de 2"<<endl;
	goto MENU;
	}}
    else if(val==true && Esqt==true){
		//caso donde la extension es .qt
		f1.open(nomArch.c_str(),ios::in);
		if(!f1.is_open()){
			cout <<"Error, el archivo no pudo ser leido"<<endl;	
			goto MENU;
		}else{
		f1>>ancho;//leo el ancho y el alto.
		f1>>alto; 
		if(EsPot2(ancho)==true && EsPot2(alto)==true && alto==ancho){//valido que sean potencias 
			//archivo ancho y alto validos.
          f1>>linea;//leo el recorrido preorden del archivo
          tam=linea.length();//se obtiene el tamaño del string en cuestion
          indice=0;
          MyTree->ArmarDesdeQt(linea,indice,tam,MyTree);//se llama para armar el arbol 
          cout<<endl;
        	f1.close();
        	goto MENU;
		}
		else{
			cout<<"Error,el ancho y el alto de la imagen no son potencia de 2"<<endl;
			cout<<endl;
			goto MENU;
		}}} 
  //archivo no valido
  cout <<"Extension del archivo invalida."<<endl;
  cout<<endl;
  goto MENU;
	  }
	  if(opcion==2){
		      if(MyTree != NULL){
			  bool qt;
			  string ruta,value;	   
			  do{
			  cout<<"Ingrese la ruta del archivo de salida(formato .qt)"<<endl;
              cin>>ruta;
			  qt=EsForqt(ruta);//funcion que verifica que sea extension .qt
			  if(qt==true){		   
              fstream f;
              f.open(ruta.c_str(),ios::out);
              f << ancho <<" "<<alto<<endl;//escribo el ancho y alto
              MyTree->SalvarImagen(MyTree,value);//llamo a salvar imagen 
              f<<value<<endl;//y escribo el valor.
              f.close();
              goto MENU;
			  }
			  if(qt==false){
				  cout<<"Extension invalida,intente de nuevo."<<endl;
			  }
		      }while(qt!=true);
		      }else{
			  cout<<"No hay imagen cargada en memoria."<<endl;
			  goto MENU;
		      }}
	       if(opcion==3){
		   if(MyTree!=NULL){
			   MyTree->NumeroColores(MyTree,B,N,G);//accion que cuenta el numero de colores de una imagen cargada en memoria 
			   H=MyTree->Altura(MyTree); //funcion que retorna la altura de un arbol
			   cout<<"B="<<B<<","<<"N="<<N<<","<<"G="<<G<<","<<"H="<<H<<endl;
			   B=0,N=0,G=0,H=0;
			   goto MENU;
		   }else{
			   cout<<"No hay imagen cargada en memoria."<<endl;
			   goto MENU;
		    }}
	          if(opcion==4){
		      if(MyTree!=NULL){
				  if(MyTree!=NULL && MyTree->Cuad0 == NULL && MyTree->color=='1'){//casos especiales en que el arbol sea homogeneo.
				 MyTree->color='0';
				  goto MENU;
				  }
				   if(MyTree!=NULL && MyTree->Cuad0 == NULL && MyTree->color=='0'){
				  MyTree->color='1';
				   goto MENU;
				  }else{
				  MyTree->InvertirColor(MyTree);//accion que cambia los colores de los nodos
				   goto MENU;
			        }
				 
				  }else{
			   cout<<"No hay imagen cargada en memoria."<<endl;
			   goto MENU;
		      }}
	   if(opcion==5){
		   if(MyTree!=NULL){
			   MyTree->RecorridoPreorden(MyTree);//funcion que muestra el recorrido preorden a partir de una imagen cargada en memoria
               cout<<endl;
               goto MENU;
		   }else{
			   cout<<"No hay imagen cargada en memoria."<<endl;
			   goto MENU;
	      }}	   
	   if(opcion==6){
		   if(MyTree!=NULL){
			   MyTree->Espejo(MyTree);//funcion que realiza en espejo 
			  goto MENU;
		   }else{
			  cout<<"No hay imagen cargada en memoria."<<endl;
			  goto MENU; 
			}}
	   if(opcion==7){
		   if(MyTree!=NULL){
			    MyTree->CrearImagen(Matriz,ancho,alto,i1,j1,MyTree);//a partir de la imagen cargada en memoria se crea la imagen
			    MyTree->MostrarMatriz(Matriz,ancho,alto,i1,j1);//y se llama a mostrar la matriz respectivamente   
			   goto MENU; 
		   }else{
			 cout<<"No hay imagen cargada en memoria."<<endl;  
			 goto MENU;
		   }}
	   if(opcion==8){
		   if(MyTree!=NULL){
	   MyTree->EliminarArbol(MyTree);//funcion que destruye el arbol
	    goto FIN;
	   }
	   goto FIN;
	   }  
	FIN:
	return 0;
}
