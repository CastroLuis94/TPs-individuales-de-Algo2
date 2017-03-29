#ifndef CONJUNTO_HPP_
#define CONJUNTO_HPP_

#include <iostream>
using namespace std;

template <class T>
class Conjunto
{
	public:

		// Constructor. Genera un conjunto vacío.
		Conjunto();

		// Destructor. Debe dejar limpia la memoria.
		~Conjunto();

		// Inserta un elemento en el conjunto. Si éste ya existe,
		// el conjunto no se modifica.
		void insertar(const T&);

		// Decide si un elemento pertenece al conjunto o no.
		bool pertenece(const T&) const;

		// borra un elemento del conjunto. Si éste no existe,
		// el conjunto no se modifica.
		void remover(const T&);

		// devuelve el mínimo elemento del conjunto según <.
		const T& minimo() const;

		// devuelve el máximo elemento del conjunto según <.
		const T& maximo() const;

		// devuelve la cantidad de elementos que tiene el conjunto.
		unsigned int cardinal() const;

		// muestra el conjunto.
		void mostrar(std::ostream&) const;

		friend ostream& operator<<(ostream& os, const Conjunto<T> &c) {
			c.mostrar(os);
			return os;
		}

	private:

		// la representación de un nodo interno.
		struct Nodo
		{
			// el constructor, toma el elemento al que representa el nodo.
			Nodo(const T& v);
			// el elemento al que representa el nodo.
			T valor;
			// puntero a la raíz del subárbol izq.
			Nodo* izq;
			// puntero a la raíz del subárbol der.
			Nodo* der; 
			
		};
		
		Nodo* raiz_;
		int cantidad;
	// funciones auxiliares
 		Nodo* inmediato(Nodo* ABorrar) const{
			Nodo* recorredor = ABorrar->izq;
			if(recorredor->der != NULL){
				while(recorredor->der != NULL){
					recorredor = recorredor->der;
				}
				return recorredor;
			}else{
				return recorredor;
			}
 		}
		
		void MostrarAux(Nodo* arbolito) const{
			if(arbolito != NULL){
				cout << arbolito->valor;
				if(arbolito->izq != NULL){
					cout << ",";
					MostrarAux(arbolito->izq); 
				}
				if(arbolito->der != NULL){
					cout << ",";
					MostrarAux(arbolito->der); 
				}
			}
		}
		bool TieneUnHijo(Nodo* padre){
			if(padre == NULL){
				return false;
			}else{
				if((padre->der != NULL and padre->izq == NULL) or ((padre->der == NULL and padre->izq != NULL))){
					return true;
				}
			}
			return false;
		}
		bool EsHoja(Nodo* padre){
			if(padre->der == NULL and padre->izq ==NULL){
				return true;
			}
			return false;
		}
		bool PuedoInsertarDerecha(Nodo* candidato,const T clave){
			if(candidato->der==NULL and candidato->valor < clave){
				return true;
			}
			return false;
		}
		
		bool PuedoInsertarIzquierda(Nodo* candidato,const T& clave){
			return candidato->izq==NULL && candidato->valor > clave;
		}
		
		Nodo* Buscar(Nodo* raiz,const T& elem) const{
			Nodo* arbolito = raiz;
			while(arbolito->valor != elem){
				if(arbolito->valor < elem){
					arbolito = arbolito ->der;
				}else{
					if(arbolito->valor == elem){
						return arbolito;
					}else{
						arbolito = arbolito -> izq;
					}
				}
			}
			return arbolito;
		}
		
		bool EsApuntado(Nodo* raiz, Nodo* ABorrar) const{
			return (raiz->valor != ABorrar->valor);
		}
		
		Nodo* ApuntaAlPadre(Nodo* raiz,Nodo* ABorrar) const{
			Nodo *recorredor = raiz;
			if(recorredor == ABorrar){
				return recorredor;
			}
			while(recorredor->der != ABorrar and recorredor->izq != ABorrar){
				if(recorredor->valor < ABorrar->valor){
					recorredor=recorredor->der;
				}else{
					recorredor=recorredor->izq;
				}
			}
			return recorredor;
		}
		
};

template <class T>
Conjunto<T>::Nodo::Nodo(const T& v)
	 : valor(v), izq(NULL), der(NULL)
{}

template <class T>
Conjunto<T>::Conjunto() : raiz_(NULL) , cantidad(0)
{}

template <class T>
Conjunto<T>::~Conjunto()
{ 
	while(raiz_ != NULL){
		remover(maximo());
//		cout <<"Cantidad de elementos:" << cantidad <<endl;
//		if(cantidad>0){
//			cout <<"Minimo: " << minimo() <<endl;
//			cout <<"Maximo: " << maximo() <<endl;
//		}
//		cout << *this <<endl;
//		cout << endl;
	}
}

template <class T>
bool Conjunto<T>::pertenece(const T& clave) const
{
	Nodo* recorredor = raiz_;
	if(cantidad ==1) {
		return raiz_->valor == clave;
	}
	if(raiz_ == NULL){
		return false;
	}else{
		while(recorredor != NULL){
			if(recorredor->valor < clave){
				if(recorredor->der == NULL){
					return false;
				}else{
					recorredor=recorredor->der;
				}
			}else{
				if(recorredor->valor == clave){
					return true;
				}
				if(recorredor->izq == NULL){
					return false;
				}else{
					recorredor=recorredor->izq;
				}
			}
		}
	}
	return false;
	
}

template <class T>
void Conjunto<T>::insertar(const T& clave){ 
	if(!(pertenece(clave))){
		if(raiz_ == NULL){
			raiz_ = new Nodo(clave);
			cantidad++;
			return;
		}else{
			Nodo* recorredor = raiz_;
			while((!PuedoInsertarDerecha(recorredor,clave) and !PuedoInsertarIzquierda(recorredor,clave)) and !EsHoja(recorredor)){
				if(recorredor->valor < clave){
					recorredor = recorredor->der;
				}else{
					recorredor = recorredor->izq;
				}
			}
			if(EsHoja(recorredor)){
				if(recorredor->valor < clave){
					recorredor->der = new Nodo(clave);
					cantidad++;
					return;
				}else{
					recorredor->izq = new Nodo(clave);
					cantidad++;
					return;
				}
			}
			if(PuedoInsertarDerecha(recorredor,clave)){
				recorredor->der = new Nodo(clave);
				cantidad++;
				return;
			}else{
				recorredor->izq = new Nodo(clave);
				cantidad++;
				return;
			}
		}
	}
}

template <class T>
unsigned int Conjunto<T>::cardinal() const
{
	return cantidad;
}

template <class T>
void Conjunto<T>::remover(const T& clave)
{	
	Nodo* ABorrar =Buscar(raiz_,clave);
	Nodo* nuevaRaiz;
	if(!EsApuntado(raiz_,ABorrar)){
		if(ABorrar->der != NULL and ABorrar->izq != NULL){
			Nodo* sucesor = inmediato(raiz_);
			Nodo* PadreDelSucesor = ApuntaAlPadre(raiz_,sucesor);
			if(PadreDelSucesor->valor == ABorrar->valor){
				raiz_ = sucesor;
				sucesor->der = ABorrar->der;
				delete(ABorrar);
				cantidad--;
				return;
			}else{
				if(sucesor->izq != NULL){
					PadreDelSucesor->der = sucesor->izq;
					sucesor->der = ABorrar->der;
					sucesor->izq = ABorrar->izq;
					delete(ABorrar);
					raiz_=sucesor;
					cantidad--;
					return;
				}else{
					PadreDelSucesor->der = NULL;
					sucesor->der = ABorrar->der;
					sucesor->izq = ABorrar->izq;
					delete(ABorrar);
					raiz_=sucesor;
					cantidad--;
					return;
				}
				
			}
		}else{
			if(ABorrar->der != NULL){
				raiz_ = raiz_->der;
				delete(ABorrar);
				cantidad--;
				return;
			}
			if(ABorrar->izq != NULL){
				raiz_ = raiz_->izq;
				delete(ABorrar);
				cantidad--;
				return;
			}
			cantidad--; 
			delete(ABorrar);
			raiz_ = NULL; 
			return;
		}
		
	}
	Nodo* PadreDelBorrado;
	if(EsApuntado(raiz_,ABorrar) and EsHoja(ABorrar)){
		PadreDelBorrado = ApuntaAlPadre(raiz_,ABorrar);
		if(PadreDelBorrado->valor < ABorrar->valor){
			PadreDelBorrado->der = NULL;
			delete(ABorrar);
			cantidad--;
			return;
		}else{
			PadreDelBorrado->izq = NULL;
			delete(ABorrar);
			cantidad--;
			return;
		}
	}
	Nodo* sucesor; 
	Nodo* PadreDelSucesor;
	if(ABorrar->der != NULL and ABorrar->izq != NULL and EsApuntado(raiz_,ABorrar)){
		sucesor = inmediato(ABorrar);
		PadreDelSucesor = ApuntaAlPadre(raiz_,sucesor);
		PadreDelBorrado = ApuntaAlPadre(raiz_,ABorrar);
		cout << clave <<endl;
		if(PadreDelSucesor->valor < ABorrar->valor){
			PadreDelSucesor->der = NULL;
		}else{
			PadreDelSucesor->izq = NULL;
		}
		if(PadreDelBorrado->valor < ABorrar->valor){
			PadreDelBorrado->der = sucesor;
		}else{
			PadreDelBorrado->izq = sucesor;
		}
		sucesor->der = ABorrar->der;
		sucesor->izq = ABorrar->izq;
		delete(ABorrar);
		cantidad--;
		return;
	}
	if(!EsApuntado(raiz_,ABorrar) and ABorrar->der != NULL){
		raiz_=ABorrar->der;
		cantidad--;
		delete(ABorrar);
		return;
	}
	
	if(ABorrar->izq != NULL and EsApuntado(raiz_,ABorrar)){
		PadreDelBorrado = ApuntaAlPadre(raiz_,ABorrar);
		if(PadreDelBorrado->valor < ABorrar->valor){
			PadreDelBorrado->der = ABorrar->izq;
		}else{
			PadreDelBorrado->izq = ABorrar->izq;
		}
		delete(ABorrar);
		cantidad--;
		return;
		
	}
	if(ABorrar->der != NULL and EsApuntado(raiz_,ABorrar)){
		PadreDelBorrado = ApuntaAlPadre(raiz_,ABorrar);
		if(PadreDelBorrado->valor < ABorrar->valor){
			PadreDelBorrado->der = ABorrar->der;
		}else{
			PadreDelBorrado->izq = ABorrar->der;
		}
		delete(ABorrar);
		cantidad--;
		return;
	}
	if(ABorrar->valor == raiz_->valor){
		if(raiz_->izq==NULL and raiz_->der !=NULL){
			raiz_=raiz_->der;
			delete(ABorrar);
			cantidad--;
			return;
		}
		if(raiz_->der==NULL and raiz_->izq !=NULL){
			raiz_=raiz_->izq;
			delete(ABorrar);
			cantidad--;
			return;
		}
		
	}

}

template <class T>
const T&  Conjunto<T>::minimo() const
{
	Nodo* recorredor = raiz_;
	while(recorredor->izq != NULL){
		recorredor = recorredor->izq;
	}
	return recorredor->valor;
}

template <class T>
const T&  Conjunto<T>::maximo() const
{
	Nodo* recorredor = raiz_;
	while(recorredor->der != NULL){
		recorredor = recorredor->der;
	}
	return recorredor->valor;
}

template <class T>
void Conjunto<T>::mostrar(std::ostream& os) const {
	cout << "[";
	MostrarAux(raiz_);
	cout << "]";
}


#endif // CONJUNTO_HPP_
