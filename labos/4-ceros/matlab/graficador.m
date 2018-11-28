function graficador(y,a,b, F_de_t, derivada_F_de_t, estimacion_deriv, seq_x_NR, seq_x_bisec)
% Dados los puntos y=[y_1,y_2,...,y_n] en R^{3 x n} y los paramentros de la
% recta a*t+b, se grafica la trayectoria h(t) del Halcon Milenario y 
% la funcion A(t) que indica el nivel de peligro

figure;

t = 0 : 0.01 : 1;
t_puntos = 0 : 0.2 : 1;
t_g = 0.5 : 0.01 : 1;

% ---- grafico de trayectoria y naves estelares
subplot(3,2,1)
plot3([b(1);a(1)+b(1)],[b(2);a(2)+b(2)],[b(3);a(3)+b(3)]) %segmento
xlabel('X');ylabel('Y');zlabel('Z');
grid on,  hold on
plot3(y(1,:)',y(2,:)',y(3,:)','.g','MarkerSize',24); % naves estelares (verde)

% ---- grafico de nivel de aturdimiento A(t)
subplot(3,2,2)
plot(t, A(t,a,b,y))
xlabel('t');ylabel('Nivel de peligro A(t)')
hold on

% ---- grafico de G
subplot(3,2,3)
G = @(t) (t - F_de_t(t) / derivada_F_de_t(t));
resG = arrayfun(G,t_g)
plot(t_g, resG)
xlabel('t');ylabel('Lo que debería ser G')
hold on

% ---- grafico de derivada de G
subplot(3,2,4)
H = @(t) (estimacion_deriv(G,t));
resG = arrayfun(H,t_g)
plot(t_g, resG)
xlabel('t');ylabel('Derivada de G')
hold on

% ---- grafico de error de newton
subplot(3,2,5)
plot(1 : length(seq_x_NR), seq_x_NR)
xlabel('t');ylabel('Pasos de newton')
hold on

% ---- grafico de error de bisoccion
subplot(3,2,6)
plot(1 : length(seq_x_bisec), seq_x_bisec)
xlabel('t');ylabel('Pasos de la bisección')
hold on

for indt = 1 : length(t_puntos),
    
    % Grafico izquierda: trayectoria de Halcon Milenario (rojo)
    subplot(3,2,1)
    plot3(a(1)*t_puntos(indt)+b(1),a(2)*t_puntos(indt)+b(2),a(3)*t_puntos(indt)+b(3),'.-r')

    % Grafico derecha: Funcion de aturdimiento A(t)
    subplot(3,2,2)
    plot(t_puntos(indt), A(t_puntos(indt), a, b, y),'.r','MarkerSize',15)
    
    % G
    %subplot(1,3,3)
    %plot(t(indt), ,'.r','MarkerSize',15)
    
    %pause(0.15);
end


 
%           .          .
%  .          .                  .          .              .
%        +.           _____  .        .        + .                    .
%    .        .   ,-~"     "~-.                                +
%               ,^ ___         ^. +                  .    .       .
%              / .^   ^.         \         .      _ .
%             Y  l  o  !          Y  .         __CL\H--.
%     .       l_ `.___.'        _,[           L__/_\H' \\--_-          +
%             |^~"-----------""~ ^|       +    __L_(=): ]-_ _-- -
%   +       . !                   !     .     T__\ /H. //---- -       .
%          .   \                 /               ~^-H--'
%               ^.             .^            .      "       +.
%                 "-.._____.,-" .                    .
%          +           .                .   +                       .
%   +          .             +                                  .
%          .             .      .        
