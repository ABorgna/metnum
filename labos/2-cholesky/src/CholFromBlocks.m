function L = CholFromBlocks(A)    
    if not(VectoresAleatorios(A, 1000))
        error('No tiene factorización de Cholesky')
    end
    if not(isequal(A,A'))
        error('A no es simétrica')
    endif
    
    % Inicializo L con ceros y Achol como auxiliar
    n = size(A,1);
    L = zeros(n,n);
    Achol = A;

    % Calculo todos los subbloques de L y U
    for i = 1:n
        L11 = sqrt(Achol(i,i));
        L21 = Achol(i+1:n, i) ./ L11;
        L22xL22t = Achol(i+1:n, i+1:n) - L21 * L21';

        L(i,i) = L11;
        L(i+1:n,i) = L21;
        Achol(i+1:n,i+1:n) = L22xL22t;
    end

    % Codigo para chequar que dio bien
    Acholesky = L*L';
    for i = 1:size(A,1)
        for j = 1:size(A,2)
            if abs(A(i,j)-Acholesky(i,j))>0.01
                error('no iguales')
            end
        end
    end
end
