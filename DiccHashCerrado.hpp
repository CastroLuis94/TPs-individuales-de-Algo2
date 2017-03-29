#ifndef DICCHASHCERRADO_H_
#define DICCHASHCERRADO_H_

#include "Lista.h"

#ifndef TAM_TABLA_INI
#define TAM_TABLA_INI 100
#endif
#ifndef UMBRAL_FC
#define UMBRAL_FC	  0.75
#endif
using namespace std;

namespace aed2 {
	
	template<class S>
	class DiccHashCerrado {
	public:
		
		typedef String K;
		
		DiccHashCerrado();
		~DiccHashCerrado();
		
		bool 	 Definido(const K& clave) const;
		void 	 Definir(const K& clave, const S& significado);
		S& 		 Significado(const K& clave);
		void 	 Borrar(const K& clave);
		Nat 	 CantClaves() const;
		
		// solo para test!!
		Lista<K> Claves() const;
		float    factorCarga() const;
		Nat      colisiones() const;
		
	private:
		
		struct TElem{
			K 	 clave;
			S 	 signif;
			
			
			TElem() : clave(), signif() {}
			
			
			TElem(const K& c, const S& v) : clave(c), signif(v) {}
		};
		void DefinirPosta(const K& clave, const S& significado);
		
		Lista<TElem>*   _tabla;
		Nat             _cant_elems;
		Nat             _tam;
		
		
		
		Nat fn_hash (const String& str) const {
			unsigned long hash = 531;
			int c = 0;
			while (c<str.size()){
				hash = ((hash << 5) + hash) + charToNat(str[c]); 
				c++;
			}
			
			return hash % _tam;
			
		}
		
		Nat charToNat(char c) const {
			return (Nat)(c);
		}
		
		
		void redimensionarTabla(){
			DiccHashCerrado<S> dhi;
			int i = 0;
			dhi._tam =_tam;
			int aux = _cant_elems;
			delete [] dhi._tabla;
			dhi._tabla = new Lista<TElem>[_tam];
			while(i<_tam){
				dhi._tabla[i] = Lista<TElem>();
				i++;
			}
			i=0;
			while(i<_tam){
				int j=0;
				while(j<_tabla[i].Longitud()){
					dhi.Definir((_tabla[i])[j].clave,(_tabla[i])[j].signif);
					j++;
				}
				
				i++;
			}
			delete [] _tabla;
			
			i=0;
			_tam = _tam*2;
			_tabla = new Lista<TElem>[_tam];
			while(i<_tam){
				_tabla[i] = Lista<TElem>();
				i++;
			}
			i=0;
			while(i<dhi._tam){
				int j=0;
				while(j<(dhi._tabla[i]).Longitud()){
					Definir((dhi._tabla[i])[j].clave,(dhi._tabla[i])[j].signif);
					j++;
				}
				
				i++;
			}
			_cant_elems = aux;
		}
		
	};
	
	/********************************************************************************
	* Implementacion
	********************************************************************************/
	
	template<class S>
	DiccHashCerrado<S>::DiccHashCerrado()
		: _cant_elems(0), _tam(TAM_TABLA_INI)
	{
		_tabla = new Lista<TElem>[TAM_TABLA_INI];
	}
	
	template<class S>
	DiccHashCerrado<S>::~DiccHashCerrado() {
		
		delete []_tabla;
	}
	
	template<class S>
	bool DiccHashCerrado<S>::Definido(const K& clave) const {
		Nat EstariaAca = fn_hash(clave);
		if(_tabla[EstariaAca].EsVacia()){
			return false;
		}else{
			int i = 0;
			while(i<_tabla[EstariaAca].Longitud()){
				if(_tabla[EstariaAca][i].clave == clave){
					return true;
				}
				i++;
			}
			return false;
		}
	}
	template<class S>
	void DiccHashCerrado<S>::DefinirPosta(const K& clave, const S& significado){
		if(Definido(clave)){
			Nat h = fn_hash(clave);
			typename Lista<TElem>::Iterador it = _tabla[h].CrearIt();
			while(it.Siguiente().clave != clave){
				it.Avanzar();
			}
			it.Siguiente().signif = significado;
		}
		else{
			Nat h = fn_hash(clave);
			typename DiccHashCerrado<S>::TElem t = TElem(clave, significado);
			_tabla[h].AgregarAtras(t);
			_cant_elems++;
		}
	}
	
	template<class S>
	void DiccHashCerrado<S>::Definir(const K& clave, const S& significado) {
		if(factorCarga() > UMBRAL_FC){
			redimensionarTabla();
			DefinirPosta(clave,significado);
		}else{
			DefinirPosta(clave,significado);
		}
	}
	
	
	template<class S>
	S& DiccHashCerrado<S>::Significado(const K& clave) {
		Nat EstariaAca = fn_hash(clave);
		typename Lista<TElem>::Iterador it = _tabla[EstariaAca].CrearIt();
		while(it.Siguiente().clave != clave){
			it.Avanzar();
		}
		return it.Siguiente().signif;
	}
	
	
	template<class S>
	void DiccHashCerrado<S>::Borrar(const K& clave) {
		Nat EstariaAca = fn_hash(clave);
		typename Lista<TElem>::Iterador it = _tabla[EstariaAca].CrearIt();
		while(it.Siguiente().clave != clave){
			it.Avanzar();
		}
		_cant_elems--;
		it.EliminarSiguiente();
	}
	
	template<class S>
	Nat DiccHashCerrado<S>::CantClaves() const {
		
		return _cant_elems;
	}
	
	// solo para test!!
	template<class S>
	Lista<typename DiccHashCerrado<S>::K> DiccHashCerrado<S>::Claves() const {
		Lista<K> ret;
		typename Lista<TElem>::const_Iterador it;
		
		
		for(Nat i=0; i < _tam; i++){
			for(it = _tabla[i].CrearIt(); it.HaySiguiente(); it.Avanzar())
				ret.AgregarAtras(it.Siguiente().clave);
		}
		
		return ret;
	}
	
	// solo para test!!
	template<class S>
	float DiccHashCerrado<S>::factorCarga() const {
		
		return float(_cant_elems)/_tam;
	}
	
	// solo para test!!
	template<class S>
	Nat DiccHashCerrado<S>::colisiones() const {
		
		Nat ret = 0;
		for(Nat i=0; i < _tam; i++){
			if (_tabla[i].Longitud() > 1)
				ret = ret + _tabla[i].Longitud() -1;
		}
		
		return ret;
	}
	
	
} /* namespace aed2 */

#endif /* DICCHASHCERRADO_H_ */
