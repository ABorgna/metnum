
% Borrar todas las variables
clear all;

% OJO: Esta linea cierra todos los graficos abiertos
close all;

% Zoom a testear (2, 3, 4...)
xZoom = 2;

% Imagenes para experimentar
orig = GenerarImagenTablero(5,5,10*xZoom);
%orig = imread('img/dubai.jpg');
%orig = imread('img/naturaleza.jpg');
orig = imread('img/circulos.png');


% Estrella de la muerte
%orig = imread('img/deathstar.jpg');


% Para pasar a escala de grises (RGB tarda mas porque hace el zoom 3 veces)
orig = rgb2gray(orig);


% Muestra la imagen para seleccionar el area. Con doble clic sobre el area
% se acepta. En 'position' devuelve la posicion [x y ancho alto] del
% rectangulo seleccionado.
[position, croppedOrig] = SeleccionarAreaParaHacerZoom(orig, xZoom);

% Se le puede pasar una posicion y esto directamente usa ese rectangulo en
% vez de mostrar la ventana de seleccion.
%[position, croppedOrig] = SeleccionarAreaParaHacerZoom(orig, xZoom, [1 1 150 147]);


Mostrar(croppedOrig, 'Original');



%% Aplica VMC
imgToZoom = Muestreo(croppedOrig, 1/xZoom, @VecinoMasCercano);
%Mostrar(imgToZoom, 'Submuestreada VecinoMasCercano');

imgVmc = Muestreo(imgToZoom, xZoom, @VecinoMasCercano);
CalidadDeImagen(imgVmc, croppedOrig, 'VecinoMasCercano');
Mostrar(imgVmc, 'VecinoMasCercano');


%% Aplica Bilineal
imgToZoom = Muestreo(croppedOrig, 1/xZoom, @Bilineal);
%Mostrar(imgToZoom, 'Submuestreada Bilineal');

imgBil = Muestreo(imgToZoom, xZoom, @Bilineal);
CalidadDeImagen(imgBil, croppedOrig, 'Bilineal');
Mostrar(imgBil, 'Bilineal');


%% Aplica Splines
imgToZoom = Muestreo(croppedOrig, 1/xZoom, @Splines);
%Mostrar(imgToZoom, 'Submuestreada Splines');

imgSpl = Muestreo(imgToZoom, xZoom, @Splines);
CalidadDeImagen(imgSpl, croppedOrig, 'Splines');
Mostrar(imgSpl, 'Spline');



