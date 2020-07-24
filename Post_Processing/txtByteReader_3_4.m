function [dataArr, tt, logging_f] = txtByteReader_2_15(filename)
%% Function info
% Version configured testing day 2/15 data collection
% Andrew Hellrigel

%Reads byte data from SD file into matlab array
    %the datastream must have this packet format:
    % [1] = LDS FL
    % [2] = LDS BL
    % [3] = Current
    % [4] = Voltage
    % [5] = Throttle Position
    % [6] = RTC Hours (0-24)
    % [7] = RTC Minutes
    % [8] = RTC Seconds
    % [9,  10] = LDS ECVT
    % [11, 12] = Primary Speed
    % [13, 14] = Secondary Speed
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
    % [35, 36] = ACCEL1 X (in m/s^2 * 100)
    % [37, 38] = ACCEL1 Y
    % [39, 40] = ACCEL1 Z
    % [41, 42] = EULER1 X
    % [43, 44] = EULER1 Y
    % [45, 46] = EULER1 Z
    % [47, 48] = GYRO1 X
    % [49, 50] = GYRO1 Y
    % [51, 52] = GYRO1 Z
    % [53, 54, 55, 56] = 0
    % [57, 58, 59, 60] = Time (Microseconds)
    % [61, 62, 63, 64] = 0
    %for a total of 64 bytes per data writing event
    
%File must be in the current directory of this function!!!!

%Outputs data array where each row corresponds to all the 13 measurements
%for each time interval, in the same order as the input stream above

%% Function body
dataSize = 64;
fr = matlab.io.datastore.DsFileReader(filename); %creat IO object
s = dir(filename);
sz60 = floor(s.bytes/dataSize);
sz = sz60*dataSize; %sz represents integer multiple of 60 less than total number of bytes
dataVec = read(fr, sz); %read in the data
dataVec = reshape(dataVec, [dataSize, sz60]); %reshape the byte data into a 60 row, sz60 sized array
    %Now, each column of DataVec is a 60 byte 'line of data'

n_items = 33;
dataArr = zeros(sz60, n_items); %initialise array for final data
    for i = 1:sz60
        %covert single byte data (10 items)
            dataArr(i,1:8) = typecast(dataVec(1:8, i), 'uint8');
        %convert unsigned 2-byte data (4 items)
            dataArr(i, 9:13) = typecast(dataVec(9:18, i), 'uint16');
        %convert signed 2-byte data (17 items)
            dataArr(i, 14:30) = typecast(dataVec(19:52, i), 'int16');
        %convert 4-byte data (2 items)
            dataArr(i, 31:33) = typecast(dataVec(53:64, i), 'uint32');
            %dataArr(i, 35) = typecast(dataVec(65:66, i), 'uint16');
    end

logging_f = sz60/((dataArr(end, 32)-dataArr(1, 32))/1e6);
%% Unit conversions
dataArr(:,3) = ((3.3*dataArr(:,3)/1023)-0.05)/0.02;
dataArr(:,14:21) = dataArr(:,14:21) / (2^14);
dataArr(:,22:30) = dataArr(:,22:30) / 100;

%Get all relevant data
dataArr = [dataArr(:,1:11), dataArr(:,14:17), dataArr(:,22:30), dataArr(:,32)];
headers1 = ["LDS_FL(mm)", 'LDS_BL(mm)', 'Current(A)', 'Voltage(V)', 'Throttle(deg)', 'Hrs', 'Min', 'Sec', 'LDS_ECVT', 'Primary(RPM)', 'Secondary(RPM)'];
headers2 = ["QuatW", 'QuatX', 'QuatY', 'QuatZ', 'AccelX(m/s^2)', 'AccelY(m/s^2)', 'AccelZ(m/s^2)', 'EulerX(rad)', 'EulerY(rad)', 'EulerZ(rad)'];
headers3 = ["GyroX(rad/s)", 'GyroY(rad/s)', 'GyroZ(rad/s)', 'Time(us)'];
headers = [headers1, headers2, headers3];

%Get only IMU data
%dataArr = [dataArr(:,15:18), dataArr(:,23:31), dataArr(:,33)];

%Get only engine data
%dataArr = [dataArr(:,11:12)];

%Get only eCVT data
%dataArr = [dataArr(:,6), dataArr(:,11:12)];

%Get only IMU and engine data
%dataArr = [dataArr(:,11:12), dataArr(:,15:18), dataArr(:,23:31), dataArr(:,33)];

%% Write to Excel

%Write the data to an excel file
% filename = "SDWritingTest3.xlsx";
% writematrix(dataArr, filename);
% xlswrite(filename, headers, "Sheet1", 'A1:BZ1');


%% Plotting
tt = 1:1/20:length(dataArr(:,1))/20 + 0.95;



end

