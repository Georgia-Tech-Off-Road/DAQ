% Copyright 2014 The MathWorks, Inc.

%% Create serial object for Arduino
s = serial('COM7', 'BaudRate', 115200); % change the COM Port number as needed
%% Connect the serial port to Arduino
s.InputBufferSize = 4; % read only one byte every time
try
    fopen(s);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end
%% Create a figure window to monitor the live data
Tmax = 15; % Total time for data collection (s)
figure,
grid on,
hold on,
xlabel ('Time (s)'), ylabel('Data (8-bit)'),
axis([0 Tmax+1 -10 300]),
%% Read and plot the data from Arduino
Ts = 0.0001; % Sampling time (s)
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
    datapacket = [0, 0, 0, 0];
    datapacket(1:4) = fread(s);
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
    % If reading faster than sampling rate, force sampling time.
    % If reading slower than sampling rate, nothing can be done. Consider
    % decreasing the set sampling time Ts
    t(i) = toc;
    if i > 1
        T = toc - t(i-1);
        while T < Ts
            T = toc - t(i-1);
        end
    end
    t(i) = toc;
    %% Plot live data
    if i > 1
        pos_line = line([t(i-1) t(i)],[pos_data(i-1) pos_data(i)]);
        set(pos_line,'Color','red');
%         DC_line = line([t(i-1) t(i)],[DC_data(i-1) DC_data(i)]);
        setpnt_line = line([t(i-1) t(i)],[setpnt_data(i-1) setpnt_data(i)]);
        set(setpnt_line,'Color','black');
        drawnow
    end
end
fclose(s);

