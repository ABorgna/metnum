function [L,U] = LUFromBlocks(A)
    n = size(A,1);
    m = size(A,2);

    % Inicializo L y U con ceros
    L = zeros(n,m);
    U = zeros(n,m);

    % El algoritmo en cuestión es in place, pero guardo una copia de A para
    % verificar el resultado al final
    Alu = A;

    if not(CheckCondLU(A))
        error('No tiene LU')
    end
    
    % Calculo todos los subbloques de L y U
    for i = 1:min(n,m)
        A12 = Alu(i, i+1:m);
        A21 = Alu(i+1:n, i);
        A22 = Alu(i+1:n,i+1:m);
        
        L11 = 1;
        U11 = Alu(i,i);
        U12 = A12;
        L21 = A21 / U11;
        LU22 = A22 - L21 * U12;

        L(i,i) = 1;
        U(i,i) = U11;
        U(i,i+1:m) = U12;
        L(i+1:n,i) = L21;
        Alu(i+1:n,i+1:m) = LU22;
    end
    % Seteo del ultimo elemento en la diagonal de L
    L(n,m) = 1;
    
    % Codigo para chequar que dio bien
    Alu = L*U;
    for i = 1:n
        for j = 1:m
            if abs(A(i,j)-Alu(i,j))>0.01
                error('no iguales')
            end
        end
    end
end

