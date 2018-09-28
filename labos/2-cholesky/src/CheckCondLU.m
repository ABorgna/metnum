function ret = CheckCondLU (a)
    % ret será
    %   1 si cumple la condición para LU
    %   0 si no
    epsilon = 1e-14;
    ret = 1;
    for i = 1:size(a,1  )
      if abs(det(a(1:i, 1:i))) < epsilon
        ret = 0;
        return
      endif
    endfor
end
