
#include "PRMatrix.h"

int main() { 
    PRMatrix<int> matrix (3, 3);
    matrix[0] = {{1,0},{2,1},{3,2}};
    matrix[1] = {{4,0},{5,1}};
    matrix[2] = {{7,0},{8,1},{9,2}};

    std::cout << matrix << std::endl;
    std::cout << matrix[1][1] << std::endl;
    return 0;
}
