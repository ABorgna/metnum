function respuesta = VectoresAleatorios(A, n)
    respuesta = true; % Simbolizando que hay confianza en que es SDP
    % Generar n vectores aleatorios y verificar condicion
    for i = 1:n
        x = rand(size(A,1),1);
        if x.' * A  * x <= 0
            respuesta = false;
            return
        end
    end
end

