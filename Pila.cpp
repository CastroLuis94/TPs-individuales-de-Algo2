typedef unsigned long Nat;
#include <iostream>
#include <ostream>
#include "Pila.h"
using namespace std ;



Pila::Pila(){
	prim = NULL;
	tam = 0;
}
Pila::~Pila(){
	while(tam>0){
		desapilar();
	}
}

void Pila::apilar(ElemPila& elem){
	Nodo* tope = new Nodo(elem);
	if (tam==0){
		tam = 1;
		prim = tope;
	}else{
		tope->sig = prim;
		prim = tope;
		tam++;
	}
}
ElemPila& Pila::tope(){
	return prim->elem;
}

void Pila::desapilar(){
	if(tam>1){
		Nodo* nuevoTope =prim->sig;
		delete(prim);
		prim = nuevoTope;
		tam--;
	}else{
		delete(prim);
		prim = NULL;
		tam = 0;
	}
}

bool Pila::esVacia(){
	return tam==0;
}


Nat Pila::tamanio() const{
	return tam;
}

Pila& Pila::operator=(const Pila & aCopiar){
	while(tam>0){
		desapilar();
	}
	Pila basura;
	basura.tam = 0;
	basura.prim = NULL;
	int i;
	i = aCopiar.tam;
	Nodo* recorredor = aCopiar.prim;
	while (i>0){
		basura.apilar(recorredor->elem);
		recorredor = recorredor->sig;
		i--;
	}
	i = aCopiar.tam;
	recorredor = basura.prim;
	while(i>0){
		apilar(recorredor->elem);
		recorredor = recorredor->sig;
		i--;
	}
	tam = aCopiar.tam;
	return *this; 
}
void Pila::mostrar(ostream& os) const{
	os << "[";
	Nodo* proximo;
	proximo = prim;
	int i=0;
	while(i<tam){
		os << (proximo->elem);
		proximo=proximo->sig;
		i++;
		if(i!=tam) os << ",";
	}
	os << "]";
	
}
