#ifndef PRMATRIX_H
#define PRMATRIX_H

#include <vector>
#include <list>
#include <iostream>

template<typename T>
class PRMatrix {
    private:
        template<typename E>
        struct Elem{
            E val;
            int col;
        };

        template<typename R>
        struct PRMatrixRow : public std::list<Elem<R>> {
            using std::list<Elem<R>>::list;

            R& operator[](int i) { 
                auto it = this->begin();
                while(it != this->end()) {
                    if (it->col == i) break;
                    it++;
                }
                return it->val;
            }

            friend std::ostream& operator<<(std::ostream &os, const PRMatrixRow<R> &row) { 
                for(auto it = row.begin(); it != row.end(); it++) {
                    os << "(" << it->val << "," << it->col << ") "; 
                }
                return os;
            }
        };
    
        std::vector<PRMatrixRow<T>> elems;
        int cant_filas;
        int cant_cols;

    public:

        PRMatrix(int cant_filas, int cant_cols) : cant_filas(cant_filas),
                                                    cant_cols(cant_cols), 
                                                    elems(std::vector<PRMatrixRow<T>>(cant_filas)) {}

        PRMatrixRow<T>& operator[](int i) { return elems.at(i);}

        friend std::ostream& operator<<(std::ostream &os, const PRMatrix<T> &matrix) {
            if (matrix.cant_filas == 0) return os;

            os << matrix.elems[0];
            for(int i=1; i < matrix.cant_filas; i++) {
                os << std::endl << matrix.elems[i];
            }
            return os;
        }
};


#endif