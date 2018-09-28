function ret = CheckCondLU (a)
    % ret será
    %   1 si cumple la condición para LU
    %   0 si no
    ret = 1;
    for i = 1:size(a,1  )
      if det(a(1:i, 1:i)) == 0
        ret = 0;
        return
      endif
    endfor
end
