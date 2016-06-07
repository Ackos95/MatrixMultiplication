function [] = ispisiMatricu( A, outputFilePath )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here

    fid = fopen(outputFilePath, 'wt'); % Open for writing
    for i=1:size(A,1)
       fprintf(fid, '%d ', A(i,:));
       fprintf(fid, '\n');
    end
    fclose(fid);

end

