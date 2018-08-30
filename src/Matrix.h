#ifndef MATRIX_H
#define MATRIX_H

#include <map>
#include <vector>
#include <iostream>
#include <assert.h>

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
		friend class Matrix<T>;
		std::map<int, T> elems;

	public:
		T operator[](int j) const{
			auto it = elems.find(j);
			if (it == elems.end()){
				return 0; // T tiene que ser casteable desde 0
			} else {
				return it->second; 
			}
		}

		void insertar(int col, T valor){
			if (valor != 0){
				elems[col] = valor;
			} else {
				elems.erase(col);
			}
		}

    // Iterators
    typedef typename std::map<int, T>::iterator iterator;
    typedef typename std::map<int, T>::const_iterator const_iterator;

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
			for (auto& elem : fila.elems){
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
				if (val != 0){
					res.insertar(i, j, val);
				}
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
				if (x != 0){
					res.insertar(i, j, x);
				}
			}
		}

		return res;
	}

	Matrix<T> operator-(const Matrix<T>& otro) const {
		return (*this)+(otro * -1.0);
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
