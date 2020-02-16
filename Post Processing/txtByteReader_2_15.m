function [dataArr, logging_f] = txtByteReader_2_15(filename)
%% Function info
% Version configured testing day 2/15 data collection
% Andrew Hellrigel

%Reads byte data from SD file into matlab array
    %the datastream must have this packet format:
    % [1] = LDS FR
    % [2] = LDS FL
    % [3] = LDS BR
    % [4] = LDS BL
    % [5] = LDS STEER
    % [6] = LDS ECVT
    % [7] = Throttle Position
    % [8] = RTC Hours (0-24)
    % [9] = RTC Minutes
    % [10] = RTC Seconds
    % [11, 12] = Engine Speed
    % [13, 14] = Wheel Speed
    % [15, 16] = Front Brake Pressure
    % [17, 18] = Back Brake Pressure
    % [19, 20] = IMU1 W
    % [21, 22] = IMU1 X
    % [23, 24] = IMU1 Y
    % [25, 26] = IMU1 Z
    % [27, 28] = IMU2 W
    % [29, 30] = IMU2 X
    % [31, 32] = IMU2 Y
    % [33, 34] = IMU2 Z
    % [35, 36] = ACCEL1 X (in g's * 100)
    % [37, 38] = ACCEL1 Y
    % [39, 40] = ACCEL1 Z
    % [41, 42] = ACCEL2 X
    % [43, 44] = ACCEL2 Y
    % [45, 46] = ACCEL2 Z
    % [47, 48] = GYRO1 X
    % [49, 50] = GYRO1 Y
    % [51, 52] = GYRO1 Z
    % [53, 54, 55, 56] = 0
    % [57, 58, 59, 60] = Time (Microseconds)
    %for a total of 60 bytes per data writing event
    
%File must be in the current directory of this function!!!!

%Outputs data array where each row corresponds to all the 13 measurements
%for each time interval, in the same order as the input stream above

%% Function body
for k = 1:62
dataSize = k;
fr = matlab.io.datastore.DsFileReader(filename); %creat IO object
s = dir(filename);
sz60 = floor(s.bytes/dataSize);
sz = sz60*dataSize; %sz represents integer multiple of 60 less than total number of bytes
dataVec = read(fr, sz); %read in the data
dataVec = reshape(dataVec, [dataSize, sz60]); %reshape the byte data into a 60 row, sz60 sized array
    %Now, each column of DataVec is a 60 byte 'line of data'

n_items = dataSize;
dataArr = zeros(sz60, n_items); %initialise array for final data
    for i = 1:sz60
        %covert single byte data (10 items)
            dataArr(i,1:dataSize) = typecast(dataVec(1:dataSize, i), 'uint8');
        %convert unsigned 2-byte data (4 items)
        %    dataArr(i, 11:14) = typecast(dataVec(11:18, i), 'uint16');
        %convert signed 2-byte data (17 items)
        %    dataArr(i, 15:23) = typecast(dataVec(19:36, i), 'int16');
        %convert 4-byte data (2 items)
        %    dataArr(i, 32) = typecast(dataVec(53:, i), 'uint32');
    end

    for i = 1:dataSize
        if dataArr(:,i) == 0;
            found = 1;
        end
    end
end
%% Unit conversions
%dataArr(:,15:22) = dataArr(:,15:22) / (2^14);
%dataArr(:,23:31) = dataArr(:,23:31) / 100;

end

