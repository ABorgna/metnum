function ret = CheckOrtogonal(A)
    epsilon = 1e-14;
    n = size(A,1);
    ret = isequal(A * A' - eye(n) < epsilon, ones(n));
end
