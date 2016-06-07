function [] = napraviTestFajl( dim, outputFilePath, outputResolvedFilePath )
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    fid = fopen(outputFilePath, 'wt'); % Open for writing
    fre = fopen(outputResolvedFilePath, 'wt');
    
    fprintf(fid, '%d\n', dim);
    
    A = randi([-100, 100], dim, dim);
    B = randi([-100, 100], dim, dim);
    C = A * B;
    
    fprintf(fre, 'A: \n');
    for i=1:size(A,1)
       fprintf(fid, '%d ', A(i,:));
       fprintf(fid, '\n');
       fprintf(fre, '%d ', A(i,:));
       fprintf(fre, '\n');
    end
    
    fprintf(fre, '\nB: \n');
    for i=1:size(B,1)
       fprintf(fid, '%d ', B(i,:));
       fprintf(fid, '\n');
       fprintf(fre, '%d ', B(i,:));
       fprintf(fre, '\n');
    end
    
    fprintf(fre, '\nC: \n');
    for i=1:size(C,1)
       fprintf(fre, '%d ', C(i,:));
       fprintf(fre, '\n');
    end
    
    fclose(fre);
    fclose(fid);

end

