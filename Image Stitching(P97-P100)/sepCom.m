function [redCom, greenCom,blueCom,image] = sepCom(data,imageWidth, imageHeight)
%SEPCOM Summary of this function goes here
%   Detailed explanation goes here
    redCom = zeros(1,imageWidth*imageHeight);
    greenCom = zeros(1, imageWidth*imageHeight);
    blueCom = zeros(1,imageWidth * imageHeight);
    for i = 1:3:(imageWidth * imageHeight *3-3)
        redCom(ceil(i/3))=data(i);
        greenCom(ceil(i/3))=data(i+1);
        blueCom(ceil(i/3))=data(i+2);    
    end
    
    redCom = reshape(redCom,imageWidth,imageHeight);
    redCom = redCom';
    greenCom = reshape(greenCom,imageWidth,imageHeight);
    greenCom = greenCom';
    blueCom = reshape(blueCom,imageWidth,imageHeight);
    blueCom = blueCom';
    image = zeros(imageHeight,imageWidth,3);
    image(:,:,1) = redCom;
    image(:,:,2) = greenCom;
    image(:,:,3) = blueCom;
end

