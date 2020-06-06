/*Fecha: 29/mayo/2020*/
/*Autores: Ana Belem Méndez Cabrera*/
/*Descripción: Creación de pila,elimina,push, revisar la cima de la  pila*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listaIndex.h"

typedef struct _quad quad;

int contadorTemporales = 0;

struct _quad{
  char op[32];
  char arg1[32];
  char arg2[32];
  char res[32];
  quad* next;
};

typedef struct _code code;

struct _code{
  quad* head;
  quad* tail;
  int num_instrucciones;
};

quad* crea_quad(char* op, char* arg1, char* arg2, char* res);
code* crea_code();
void elimina_quad(quad* q, code* c);
void elimina_code(code* c);
void agrega_cuadrupla(code* c, char* op, char* arg1, char* arg2, char* res);
void agrega_cuadrupla_existente(quad* q, code* c);
quad *buscarQuadIndi(code *codigo, char *indice);
void backpach(code *codigo, listIndex *listIndice, char *etiqueta);
void imprimirCodigo(code *codigo);

char *reduccion (int tipo, int tipoRedu,char* dir, code* codigo);
char *ampliacion (int tipo, int tipoAmpl,char* dir, code* codigo);
int max (int tipo_1, int tipo_2);
char* newTemp();

//===============================================================================

/*Crea una nueva cruádrupla*/
quad* crea_quad(char* op, char* arg1, char* arg2, char* res){
  quad* new_quad = (quad*)malloc(sizeof(quad));
  strcpy(new_quad->op, op);
  strcpy(new_quad->arg1, arg1);
  strcpy(new_quad->arg2, arg2);
  strcpy(new_quad->res, res);
  new_quad->next  = NULL;
  return new_quad;
}

/*Reserva memoria para una nueva sección de código*/
code* crea_code(){
  code* new_code = (code*)malloc(sizeof(code));
  new_code->num_instrucciones = 0;
  new_code->head = NULL;
  new_code->tail = NULL;
  return new_code;
}

/*Elimina la cruádrupla dada del código indicado
  si la cuadrupla no existe en el código no hace nada*/
void elimina_quad(quad* q, code* c){
	quad* ant_busq = NULL;
	quad* act_busq = c->head;
	while (act_busq == NULL || act_busq != q ) {
  	ant_busq = act_busq;
  	act_busq = act_busq->next;
	}
	if (act_busq == NULL)
  	return;
	if(ant_busq == NULL){
  	c->head = NULL;
	}else{
  	ant_busq->next = act_busq->next;
	}
	c->num_instrucciones--;
	free(q);
}

/*Libera la memoria del código y todos sus elementos*/
void elimina_code(code* c){
  quad* borrarP;
  quad* borrarH = c->head;
  while(borrarH != NULL){
	borrarP = borrarH;
	borrarH = borrarH->next;
	free(borrarP);
  }
  free (c);
}

/*Crea y agrega una cuadrupla al final del código
  Retorna la cuadrupla agregada*/
void agrega_cuadrupla(code* c, char* op, char* arg1, char* arg2, char* res){

 
  quad* new_quad =  crea_quad(op, arg1, arg2, res);

  if (c->num_instrucciones == 0) {
	c->tail = new_quad;
	c->head 	= new_quad;
  }else{
	c->tail->next = new_quad;
	c->tail   	= new_quad;
  }

  c->num_instrucciones++;
}

/*Agrega una cuádrupla declarada anteriormente al final del código */
void agrega_cuadrupla_existente(quad* q, code* c){
  if (c->num_instrucciones == 0) {
	c->tail = q;
	c->head 	= q;
  }else{
	c->tail->next = q;
	c->tail   	= q;
  }
  c->num_instrucciones++;
}


quad *buscarQuadIndi(code *codigo, char *indice){
  quad *q;
  q = codigo->head;

  while(q!=NULL){
  	if (strcmp(q->res,indice)==0){
      	return q;}
  	q = q->next;
  }
 
   printf ("Codigo: El indice %s no esta en el codigo", indice);
   return NULL;
 

 

}


void backpach(code *codigo, listIndex *listIndice, char *etiqueta){
  quad* q = (quad*)malloc(sizeof(quad));
  myIndex *indi;
  char *tmp=(char*)malloc(sizeof(char));
 
  if(listIndice!=NULL){
  indi = listIndice->head;
  while(indi != NULL){
	sprintf(tmp, "%i", indi->valorMyIndex);
	q = buscarQuadIndi(codigo,tmp);
	if (q!=NULL){
  	strcpy(q->res, etiqueta);
 	 
	}
	indi = indi->sig;
   }
  }
}




//============================================================================
char *reduccion (int tipo, int tipoRedu, char* dir, code* codigo){


  char* newDir=(char*)malloc(sizeof(char));
  char *car = "(CAR)";
  char *real = "(REAL)";
  char *ent = "(ENT)";
  char *iqual = " ";

  if (tipo == tipoRedu) {
	newDir = dir;
  } else if (tipo == 4 && tipoRedu == 1) { /*Reducción a real*/
	newDir = newTemp();
	agrega_cuadrupla(codigo, real,iqual, dir,newDir);
  } else if ((tipo == 4 || tipo == 1) && tipoRedu == 0) { /*Reducción a ent*/
	newDir = newTemp();
	agrega_cuadrupla(codigo, ent,iqual, dir,newDir);
  } else if ((tipo == 4 || tipo == 1 || tipo == 0) && tipoRedu == 2) { /*Reducción a carac*/
	newDir = newTemp();
	agrega_cuadrupla(codigo, car,iqual, dir,newDir);
  } else {
  printf("ERROR:No es posible hacer la reducción\n");
	newDir =NULL;
  }
  return newDir;
}

// 0 = ent, 1= real, 4=dreal, 2=car
char *ampliacion (int tipo, int tipoAmpl, char* dir, code* codigo){ //DIRECCION DEL DATO AMPLEADO
   char* newDir=(char*)malloc(sizeof(char));
   char *dreal = "(DREAL)";
   char *real = "(REAL)";
   char *ent = "(ENT)";
   char *iqual = " ";
  if (tipo == tipoAmpl){
	newDir = dir;
  } else if (tipoAmpl == 4 && (tipo == 0 || tipo == 1 || tipo == 2)){ /*Amplia a dreal*/
	newDir = newTemp();
	agrega_cuadrupla(codigo, dreal,iqual,dir,newDir);
  } else if (tipoAmpl == 1 && (tipo == 0 || tipo == 2 )){ /*Amplia a real*/	 
	newDir = newTemp();
	agrega_cuadrupla(codigo, real,iqual, dir,newDir);
  } else if (tipoAmpl == 0 && tipo == 2){ /*Amplia a ent*/
	newDir = newTemp();
	agrega_cuadrupla(codigo, ent,iqual, dir, newDir);
  }else{
	printf("ERROR: No es posible hacer la ampliación\n");
	newDir = NULL;
  }
 
  return newDir;
}

/*
* Función: max
* Descripción: Retorna el tipo de dato más grande, en caso de no encontrarlo retorna un -1*/
int max (int tipo_1, int tipo_2){
  code *myCode;
  myCode = crea_code();
  //dreal, real, ent, car
  int tipos[] = {4,1,0,2};
  for (int i = 0; i < 4; i++) {
	if(tipo_1 == tipos[i] || tipo_2 == tipos[i])
  	return tipos[i];
  }
  printf("Los tipos no son compatibles");
  return -1;
}

/* Función: newTemp
* Descripción: Reserva memoria y retorna la diracció de inicio de la nueva variable en caso de no encontrarlo retorna un -1*/
char* newTemp(){
  contadorTemporales++;
   char *n=(char*)malloc(sizeof(char));
  char *tem=(char*)malloc(sizeof(char));
  strcpy(n,"tmp");
  sprintf(tem, "%i", contadorTemporales);
  strcat(n,tem);
  return n;
}


void imprimirCodigo(code *codigo){
  int tamanio, i, aux;
  if(codigo!=NULL){
	printf("\n================================================================\n");
	printf("|               	CODIGO DE TRES DIRECCIONES             	|\n");
	printf("================================================================\n");
	quad *q=codigo->head;
	while (q!= NULL){
  	if(strcmp(q->res, " ") ){
    	tamanio = strlen(q->res);
    	printf("(%s)", q->res);
    	for (i=tamanio; i<9; i++)
      	printf(" ");
    	printf("= arg1:(%s) operador:(%s) arg2:(%s)\n",q->arg1,q->op,q->arg2);
  	}
  	else {
    	aux = 9 - tamanio;
    	if(aux>0){
      	aux = tamanio + aux + 4;
      	for (i=0; i<aux; i++)
        	printf(" ");
      	printf("arg1:(%s) operador:(%s) arg2:(%s)\n",q->arg1,q->op,q->arg2);
    	}else{
      	for (i=0; i<tamanio + 4; i++)
        	printf(" ");
      	printf("arg1:(%s) operador:(%s) arg2:(%s)\n",q->arg1,q->op,q->arg2);
    	}
  	}
  	q=q->next;
	}
  }


}
