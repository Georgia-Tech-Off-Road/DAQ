function [dataArr, logging_f] = txtByteReader_mm2019(filename)

% Version configured for midnight mayhem 2019 data collection
% Billy Plummer

%Reads byte data from SD file into matlab array
    %the datastream must have this packet format:
    % [1, 2] = LDS0 
    % [3, 4] = LDS1
    % [5, 6] = LDS2
    % [7, 8] = LDS3
    % [9, 10] = acc1_x
    % [11, 12] = acc1_y
    % [13, 14] = acc1_z
    % [15, 16] = acc2_x
    % [17, 18] = acc2_y
    % [19, 20] = acc2_z
    % [21, 22] = f_axle
    % [23, 24, 25, 26] = Time
    %for a total of 26 bytes per data writing event
    
%File must be in the current directory of this function!!!!

%Outputs data array where each row corresponds to all the 13 measurements
%for each time interval, in the same order as the input stream above

fr = matlab.io.datastore.DsFileReader(filename); %creat IO object
s = dir(filename);
sz26 = floor(s.bytes/26);
sz = sz26*26; %read size to nearest 26 byte unit
dataVec = read(fr, sz); %create vector of raw byte
dataVec = reshape(dataVec, [26, sz26]);

dataArr = zeros(sz26, 12);
    for i = 1:sz26
            dataArr(i,1:11) = typecast(dataVec(1:22, i), 'uint16');%measurements
            dataArr(i, 12) = typecast(dataVec(23:26, i), 'uint32');%time
    end
logging_f = sz26/((dataArr(end, 12)-dataArr(1, 12))/1e6); %Hz
dataArr(:, 5:10) = (dataArr(:, 5:10)-2048)*0.076645;
end

