% Datos: polinomio de grado 2 (con ruido).
t = [1:0.1:40];
y = 1/2 * t ** 2 + 4 * t + 15;
y = y + np.random.randn(*x.shape) * 10;  # Agrego ruido
figure
plot(y)
title("Datos a ajustar")

% Ecuaciones normales de cuadrados minimos
% Vamos a fitear por un polinomio de grado 4
A = [ones(size(t) t' t.^2' t.^3' t.^4'];
b = y';
AtA = A' * A;
Atb = A' * b;

% El sistema a resolver es: AtA x = Atb
correct_ans = AtA \ Atb;

% Gauss-Seidel
D = diag(diag(AtA));
L = -tril(AtA, -1);
U = -triu(AtA, 1);

% COMPLETAR
T = _;
c = _;
% AJUSTAR PARAMETRO REPS
reps = 100000;
% COMPLETAR
xi = _;
error = zeros(1, reps);
for i = 1:reps
  % COMPLETAR
  xi = _;
  error(i) = norm(xi - correct_ans);
end
printf("Gauss-Seidel:")
xi
correct_ans
figure;
plot(error)
title("Error de Gauss-Seidel")

figure
hold on
plot(y)
pred = xi(1) * t + xi(2) * t**2 + xi(3) * t**3 + xi(4) * t**4;
plot(pred, 'r')
title("Polinomio ajustado")