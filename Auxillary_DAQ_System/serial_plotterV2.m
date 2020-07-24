%% Create serial object for Arduino
s = serial('COM7', 'BaudRate', 115200); % change the COM Port number and Baud Rate as needed
%% Connect the serial port to Arduino
s.InputBufferSize = 4; % number of data items in array
try
    fopen(s);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end
%% Create a figure window to monitor the live data
Tmax = 20; % Total time for data collection (s)
figure
hold on,
pos_line = animatedline('Color', 'r');
xlabel ('Time (s)'), ylabel('Data (8-bit)'),
axis([0 Tmax+1 -10 300]),
grid on
setpnt_line = animatedline('Color', 'b');
%% Read and plot the data from Arduino
i = 0;
pos_data = 0;
DC_mag_data = 0;
DC_sign_data = 0;
DC_data = 0;
setpnt_data = 0;
t = 0;
tic % Start timer
while toc <= Tmax
    i = i + 1;
    %% Read buffer data
    datapacket = zeros(4); % Set length to be number of data items
    datapacket(1:4) = fread(s, 'double');
    pos_data(i) = datapacket(1);
    DC_mag_data(i) = datapacket(2);
    DC_sign_data(i) = datapacket(3);
    setpnt_data(i) = datapacket(4);
    if DC_sign_data(i) == 0
        sign = -1;
    else
        sign = 1;
    end
    DC_data(i) = sign*DC_mag_data(i);
    %% Read time stamp
    t = toc;
    addpoints(pos_line, t, pos_data(i));
    addpoints(setpnt_line, t, setpnt_data(i));
    drawnow;

end
fclose(s);