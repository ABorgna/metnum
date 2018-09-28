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
        L11 = ...;
        U11 = ...;
        U12 = ...;
        L21 = ...;
        LU22 = ...;

        L(i,i) = ...;
        U(i,i) = ...;
        U(i,i+1:m) = ...;
        L(i+1:n,i) = ...;
        Alu(i+1:n,i+1:m) = ...;
    end
    % Seteo del ultimo elemento en la diagonal de L
    L(n,m) = 1;
    
    % Codigo para chequar que dio bien
    Alu = L*U;
    for i = n
        for j = m
            if abs(A(i,j)-Alu(i,j))>0.01
                error('no iguales')
            end
        end
    end
end

