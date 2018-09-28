function [Lch, correct] = CholFromLU(A)
    if not(VectoresAleatorios(A, 1000))
        error('No tiene factorización de Cholesky')
    end
    epsilon = 0.00001;

    [L,U] = LUFromBlocks(A);
    
    % Conseguir L de cholesky a partir de LU
    Lch = ...;  

    if abs(sum(sum(A - Lch * Lch'))) >= epsilon
        correct = 0;
        return
    end
    
    correct = 1;
end

