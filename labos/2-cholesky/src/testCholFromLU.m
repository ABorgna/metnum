function [pass] = testCholFromLU()
    n = 50;
    rand('seed', 42)
    for i = 1:n
        A = rand(15);
        A = A * A';
        [Lch, correct] = CholFromLU(A);
        if correct
            fprintf('Anda :D \n')
            fprintf('con un error de:\n')
            abs(sum(sum(A - Lch * Lch')))
            return
        end
    end
    fprintf('No anda D:\n');
end

