function data = readraw(filename)
%readraw - read RAW format grey scale image of square size into matrix G
% Usage:	G = readraw(filename)

	disp(['	Retrieving Image ' filename ' ...']);

	% Get file ID for file
	fid=fopen(filename,'rb');

	% Check if file exists
	if (fid == -1)
	  	error('can not open input image file press CTRL-C to exit \n');
	  	pause
	end

	% Get all the pixels from the image
	data = fread(fid, inf, 'uchar');

	% Close file
	fclose(fid);

end %function
