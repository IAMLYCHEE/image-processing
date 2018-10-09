clear;
clc;
imageWidth = 480;
imageHeight = 640;

dataleft = readraw('left.raw');
[rl,gl,bl,Il] = sepCom(dataleft,imageWidth,imageHeight);

data = readraw('middle.raw');
[r,g,b,I] = sepCom(data,imageWidth,imageHeight);

dataright = readraw('right.raw');
[rr,gr,br,Ir]= sepCom(dataright,imageWidth,imageHeight);


%preprocess for brighter and higher contrast
rr = preprocess(rr);
gr = preprocess(gr);
br = preprocess(br);

adjustl = 59;
adjustr = 100;
panoedr = pano(rl,r,rr,imageWidth,imageHeight,adjustl,adjustr);
panoedg = pano(gl,g,gr,imageWidth,imageHeight,adjustl,adjustr);
panoedb = pano(bl,b,br,imageWidth,imageHeight,adjustl,adjustr);

planesize = size(panoedr);

image = zeros(planesize(1),planesize(2),3);
image(:,:,1) = panoedr;
image(:,:,2) = panoedg;
image(:,:,3) = panoedb;

imshow((uint8(image)));
