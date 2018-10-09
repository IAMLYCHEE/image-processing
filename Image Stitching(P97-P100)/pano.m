function panoed = pano(rl,r,rr,imageWidth,imageHeight,adjustl,adjustr)
planeWidth = imageWidth * 2.2;
planeHeight = imageHeight * 1.2;

planeR = zeros(planeHeight,planeWidth);
hoffset = (planeWidth - imageWidth)/2;
voffset = (planeHeight - imageHeight)/2;

%add middle
% for i = voffset+1 : voffset  + imageHeight
%     for j = hoffset+1 : hoffset  + imageWidth
%         planeR(i,j) = r(i-voffset,j-hoffset);
%     end
% end




imageHeight = 640;

%first four points trial
% leftP = [472,372;  563,374;  466,276;  558,277];
% middleP = [467,117;  559,119;  477,15;  571,17];

%second four points trial
leftP = [472,421;  32,321;  466,276;  195,325];
middleP = [462,164;  19,58;  477,15;  189,66];

%
middlePPlane = [middleP(:,1) + voffset,middleP(:,2) + hoffset];

%get the catesian coordinate
p = [leftP(:,2)-0.5 , imageHeight - leftP(:,1)+0.5]; %left point in image's coordinate
pPrime = [middlePPlane(:,2)-0.5 , planeHeight - middlePPlane(:,1)+0.5];%middle point in plane's coordinate

C = [p(1,1),p(1,2),1,0,0,0,0,0,0;
    0,0,0,p(1,1),p(1,2),1,0,0,0;
    0,0,0,0,0,0,p(1,1),p(1,2),1;
    p(2,1),p(2,2),1,0,0,0,0,0,0;
    0,0,0,p(2,1),p(2,2),1,0,0,0;
    0,0,0,0,0,0,p(2,1),p(2,2),1;
    p(3,1),p(3,2),1,0,0,0,0,0,0;
    0,0,0,p(3,1),p(3,2),1,0,0,0;
    0,0,0,0,0,0,p(3,1),p(3,2),1;
    p(4,1),p(4,2),1,0,0,0,0,0,0;
    0,0,0,p(4,1),p(4,2),1,0,0,0;
    0,0,0,0,0,0,p(4,1),p(4,2),1];

Homo = [pPrime(1,1);pPrime(1,2);1; pPrime(2,1);pPrime(2,2);1;pPrime(3,1);pPrime(3,2);1;pPrime(4,1);pPrime(4,2);1];


% transT = linsolve(C,Homo);
transT = C\Homo;
transT = reshape(transT,3,3);
transT = transT';
transTinv = inv(transT);
%now get the boundary for mapping
augCorner = [[0.5;0.5;1],[0.5;imageHeight - 0.5;1],[imageWidth - 0.5;0.5;1],[imageWidth-0.5;imageHeight-0.5;1]];
%boundary for the plane
cornerPlane = transT*augCorner;
minX = min(cornerPlane(1,:));
minY = min(cornerPlane(2,:));
maxX = max(cornerPlane(1,:));
maxY = max(cornerPlane(2,:));

jStart = int16(planeHeight-maxY + 0.5);
jend = int16(planeHeight - minY + 0.5);
kStart = int16(minX +0.5);
kend = int16(maxX +0.5);

for j = jStart : jend
    for k = kStart :kend
        pafter = [double(k)-0.5;planeHeight-double(j)+0.5;1];
        pbefore = transTinv * pafter;
        ij = int16(imageHeight - pbefore(2) + 0.5);
        ik = int16(pbefore(1) + 0.5);
        if(ij >0 && ij<=640 && ik>0 &&ik<=480)
            planeR(j,k) = rl(ij,ik);
        end
    end
end




%operations for the right first trial
rightP = [492,189;  568,193;  568,138;  492,134];
middleP = [499,386;  578,390;  569,333;  494,329];
%second trial
% rightP = [195,52;567,138;548,61;525,258];
% middleP = [201,253;569,333;538,253;544,462];
middlePPlane = [middleP(:,1) + voffset,middleP(:,2) + hoffset];

%get the catesian coordinate
p = [rightP(:,2)-0.5 , imageHeight - rightP(:,1)+0.5]; %left point in image's coordinate
pPrime = [middlePPlane(:,2)-0.5 , planeHeight - middlePPlane(:,1)+0.5];%middle point in plane's coordinate

C = [p(1,1),p(1,2),1,0,0,0,0,0,0;
    0,0,0,p(1,1),p(1,2),1,0,0,0;
    0,0,0,0,0,0,p(1,1),p(1,2),1;
    p(2,1),p(2,2),1,0,0,0,0,0,0;
    0,0,0,p(2,1),p(2,2),1,0,0,0;
    0,0,0,0,0,0,p(2,1),p(2,2),1;
    p(3,1),p(3,2),1,0,0,0,0,0,0;
    0,0,0,p(3,1),p(3,2),1,0,0,0;
    0,0,0,0,0,0,p(3,1),p(3,2),1;
    p(4,1),p(4,2),1,0,0,0,0,0,0;
    0,0,0,p(4,1),p(4,2),1,0,0,0;
    0,0,0,0,0,0,p(4,1),p(4,2),1];

Homo = [pPrime(1,1);pPrime(1,2);1; pPrime(2,1);pPrime(2,2);1;pPrime(3,1);pPrime(3,2);1;pPrime(4,1);pPrime(4,2);1];


% transT = linsolve(C,Homo);
transT = C\Homo;
transT = reshape(transT,3,3);
transT = transT';
transTinv = inv(transT);
%now get the boundary for mapping
augCorner = [[0.5;0.5;1],[0.5;imageHeight - 0.5;1],[imageWidth - 0.5;0.5;1],[imageWidth-0.5;imageHeight-0.5;1]];
%boundary for the plane
cornerPlane = transT*augCorner;
minX = min(cornerPlane(1,:));
minY = min(cornerPlane(2,:));
maxX = max(cornerPlane(1,:));
maxY = max(cornerPlane(2,:));

jStart = int16(planeHeight-maxY + 0.5);
jend = int16(planeHeight - minY + 0.5);
kStart = int16(minX +0.5);
kend = int16(maxX +0.5);

for j = jStart : jend
    for k = kStart :kend
        pafter = [double(k)-0.5;planeHeight-double(j)+0.5;1];
        pbefore = transTinv * pafter;
        ij = int16(imageHeight - pbefore(2) + 0.5);
        ik = int16(pbefore(1) + 0.5);
        if(ij >0 && ij<=640 && ik>0 &&ik<=480)   
               planeR(j,k) =  rr(ij,ik);%brighter
        end
    end
end





for i = voffset+1 : voffset  + imageHeight
    for j = hoffset+1+adjustl : hoffset  + imageWidth-adjustr
        planeR(i,j) = r(i-voffset,j-hoffset);
    end
end


panoed = planeR;


end