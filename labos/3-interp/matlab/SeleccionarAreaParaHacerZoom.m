function [position, croppedImg] = SeleccionarAreaParaHacerZoom(img, xZoom, position)
    if nargin == 2,
        hfigSeleccion = figure('name', 'Seleccionar area');
        imshow(img);

        % Habilita la selección de un rectángulo
        h = imrect;

        % Restricciones del rectangulo
        setPositionConstraintFcn(h, (@(newPos) RestriccionesDeRectangulo(newPos, xZoom, size(img))));

        % Esperar a que haga doble click sobre el rectángulo
        wait(h);

        % Obtener posición del rectángulo
        position = h.getPosition;
        
        close(hfigSeleccion);
    end

    % Obtener el cuadradito de la imagen original que le vamos a hacer zoom
    croppedImg = img(position(2):(position(2)+position(4)-1), position(1):(position(1)+position(3)-1), :);
end