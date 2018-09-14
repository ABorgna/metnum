#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <assert.h>
#include <iostream>
#include <limits>
#include <map>
#include <vector>

template<typename T>
class Matrix{
/* Clase para representar matrices ralas
 * 
 * Mantiene un vector con cant_filas de elementor, donde cada elemento representa una fila,
 * y cada fila se representa como un map<int, elem>, donde acceder al elemento k, significa 
 * ver el elemento en la columna k. Si no esta definido, se retorna 0.
 * 
 * Acceder a una fila cuesta O(1), y a un elemento de la fila i O(log(#elementosDistintosDeCero(i)))
 */
public:
	class Fila {
    typedef std::vector<std::pair<int, T>> Container;
		Container elems;

	public:
    // Iterators
    typedef typename Container::iterator iterator;
    typedef typename Container::const_iterator const_iterator;

		const_iterator find(int j) const {
      std::pair<int, T> target = {j, std::numeric_limits<T>::lowest()};
			auto it = lower_bound(elems.begin(), elems.end(), target);
			if (it == elems.end() || it->first > j){
				return elems.end();
			} else {
				return it;
			}
		}

		T operator[](int j) const {
			auto it = find(j);
			if (it == elems.end()){
				return 0; // T tiene que ser casteable desde 0
			} else {
				return it->second;
			}
		}

    // Inserción de elementos en las columnas.
    //
    // Insertar en una columna posterior a todas las actuales es O(1)
    // Insertar en una columna arbitraria es O(n)
		void insertar(int col, T valor){
      const T EPSILON = 1e-4;
      const bool isZero = -EPSILON <= valor && valor <= EPSILON;

      if(elems.empty() || col > elems.back().first) {
        if(!isZero) elems.push_back({col, valor});
			} else if (isZero){
        // Borrar una columna si está presente
        auto it = find(col);
        if(it != elems.end()) elems.erase(it);
      } else {
        // Agregar o editar una columna
        // Buscar el lugar de inserción
        std::pair<int, T> target = {col, std::numeric_limits<T>::lowest()};
        auto it = lower_bound(elems.begin(), elems.end(), target);

        if(it != elems.end() && it->first == col) {
          // Editarlo si ya está presente
          it->second = valor;
        } else {
          // Insertarlo si no está
          elems.insert(it, {col, valor});
        }
      }
		}

    iterator begin() noexcept {
      return elems.begin();
    }

    const_iterator begin() const noexcept {
      return elems.begin();
    }

    iterator end() noexcept {
      return elems.end();
    }

    const_iterator end() const noexcept {
      return elems.end();
    }
	};
private:
	int num_fil, num_col;
	std::vector<Fila> filas;

public:
	Matrix(int filas, int columnas) : num_fil(filas), num_col(columnas), filas(filas) {};

	int num_filas() const {
		return num_fil;
	}

	int num_columnas() const {
		return num_col;
	}

	Fila& operator[](int i){
		return filas[i];
	}

	const Fila& operator[](int i) const {
		return filas[i];
	}

	void insertar(int fil, int col, T valor){
		return filas[fil].insertar(col, valor);
	}

	Matrix<T> operator*(T escalar) const {
		Matrix<T> res = *this;
		for (auto& fila : res.filas){
			for (auto& elem : fila){
				elem.second *= escalar;
			}
		}

		return res;
	}

	Matrix<T> operator*(const Matrix<T>& otro){
		assert(num_col == otro.num_filas());

		Matrix<T> res(num_fil, otro.num_columnas());

		for (int i = 0; i < num_fil; ++i){
      const auto& fila = (*this)[i];

			for (int j = 0; j < otro.num_columnas(); ++j){
				T val = 0;

        for(auto p : fila) {
          int k = p.first;
          T x = p.second;
					val += x * otro[k][j];
        }
				res.insertar(i, j, val);
			}
		}

		return res;
	}

	Matrix<T> operator+(const Matrix<T>& otro) const {
		assert(num_fil == otro.num_filas());
		assert(num_col == otro.num_columnas());

		Matrix<T> res(num_fil, num_col);
		for (int i = 0; i < num_fil; ++i){
			for (int j = 0; j < num_col; ++j){
				T x = (*this)[i][j] + otro[i][j];
				res.insertar(i, j, x);
			}
		}

		return res;
	}

	Matrix<T> operator-(const Matrix<T>& otro) const {
    auto negOtro = otro * -1.0;
    auto res = *this + negOtro;
		return res;
	}

	

	friend std::ostream& operator<<(std::ostream &os, const Matrix<T> &matrix) {
		for (int i = 0; i < matrix.num_filas(); ++i){
			for (int j = 0; j < matrix.num_columnas(); ++j){
				os << matrix[i][j] << ' ';
			}
			os << std::endl;
		}

        return os;
    }


};

template<typename T>
Matrix<T> operator*(T escalar, const Matrix<T>& m) {
	return m*escalar;	
}

template<typename T>
Matrix<T> identidad(int n){ // devuelve la matriz I de nxn
	Matrix<T> res(n, n);
	for (int i = 0; i < n; ++i){
		res.insertar(i, i, 1);
	}

	return res;
}

#endif  // MATRIX_H
