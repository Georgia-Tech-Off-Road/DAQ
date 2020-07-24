%% Creating serial object (only run once)
teensy = serial('COM5', 'BaudRate', 115200); %match with Arduino
teensy.InputBufferSize = 2; %data packets are 16 bytes long, no need to make this any bigger
%adjust port, BR as necessary
%% Testing connection
try
    fopen(teensy);
catch err
    error('could not open serial connection, make sure port, BR are correct');
end
%try and open the connection
pause(1);
%wait for things to settle
fclose(teensy);
disp('test connection successful');
%% re-run section
clc;
close all;
fopen(teensy);

rxArray = [];

totTime = input('Enter time in seconds to collect data. DAQ will begin immediately: \n');
timeData = [];
tic; %start clock
while toc < totTime
    rxPacket(1:2) = fread(teensy);
    rxArray = [rxArray; rxPacket];
    timeData = [timeData; toc];
end
fclose(teensy);
disp('Data has been collected...');

[dataSize, ~] = size(rxArray); %number of rows of data
approxF = dataSize/totTime;
noOfToneWheelTeeth = 20;
scaleFactor = 60/noOfToneWheelTeeth;

rotationalSpeed_rpm = zeros(dataSize, 1);


for i = 1:dataSize
    rotationalSpeed_rpm(i) = double(typecast(uint8(rxArray(i, 1:2)), 'uint16'))*scaleFactor;
end
figure
plot(timeData, rotationalSpeed_rpm);
xlabel('time (s)');
ylabel('Speed (RPM)');
grid on;
