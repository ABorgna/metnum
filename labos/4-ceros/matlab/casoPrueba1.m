%% --------- CASO PRUEBA 1 ------------------
% a,b= parametros de ecuacion de la recta
% y  = puntos en R^{3 x n} donde se ubican las rocas de kriptonita
% C  = cota del valor de aturdimiento 
% F  = funcion a encontrar la raiz
% derivada_F = derivada de F
%
close all  %cierra todas las ventanas
clear variables  %elimina variables del entorno

%% CASO 1
%
% Definiciones 

%parametros de la recta
a = [3;1;1];
b = [1;1;-0.5];

% coordenadas de las naves estelares
y = [ [2;3;-0.5] , [2;0.5;0.5] ];   

% cota para el nivel de aturdimiento
C = 1;

% Definicion de Funciones

 F = @(t,a,b,y,C)(A(t,a,b,y) - C);
 derivada_F = @(t,a,b,y,C) derA(t,a,b,y);

 F_de_t = @(t) F(t,a,b,y,C);
 derivada_F_de_t = @(t) derivada_F(t,a,b,y,C);


%% NEWTON

disp('NEWTON-RAPHSON')

x0 = 0.51;
tol = 0.00000001; 
maxnumit = 40;

[xfinal,seq_x_NR] = NewtonR(F_de_t, derivada_F_de_t, tol, maxnumit, x0);

%% BISECCION

disp('BISECCION')

x0 = 0;
x1 = 1;
tol = 0.00000001; 
maxnumit = 40;

[xfinal,seq_x_Bisec] = Biseccion(F_de_t, tol, maxnumit, x0, x1);

% Grafico de la funcion de aturdimiento

est = @(f,a) (Estimacion_derivada(f,a))
graficador(y,a,b, F_de_t, derivada_F_de_t, est, seq_x_NR, seq_x_Bisec)

