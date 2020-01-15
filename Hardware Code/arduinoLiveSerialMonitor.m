%% Live Serial Monitor for Arduino (or any other serial device)

% Billy Plummer (bplummer6@gatech.edu) based on pre-existing MATLAB
% function

%*******INSTRUCTIONS**********************

% Arduino code should be of this form:
%         byte item1 = (byte)somedata1;
%         byte item2 = (byte)somedata2;
%         ...
%         byte itemN = (byte)somedataN;
%         byte array[N] = {item1, item2,...,itemN};
%         Serial.write(array, N);
%         //Where N is the number of data items in total, all data must be
%         type byte
% Notes:
    %1) this only works for 8-bit (single byte) data right now, you may
    %need to map 'somedata' items to unsigned 8-bit (0-255) range.
    %2) to send signed data, use some logical structure in the arduino code
    %to send a sign flag. For this code, if it receives a 0 in the index
    %designated to sign data, it multiplies the relevant data by -1, and
    %vice versa for receiving a 1.
    %3) the sampling time, Ts has a practical limit around 0.0001, going
    %lower makes little difference, going higher slows the sampling
    %resolution. Sampling frequency cannot be higher than the data writing
    %frequency from the arduino itself.
    %4) to check the COM port number, go into the arduino IDE, then
    %tools>port and see what is connected.
    %5) don't forget to update the number of items per reading across the
    %whole MATLAB code!
    %6) more data items may be plotted by copying the form of the lines in
    %te 'plot live data' section at the bottom.

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
figure,
grid on,
hold on,
xlabel ('Time (s)'), ylabel('Data (8-bit)'),
axis([0 Tmax+1 -10 300]),
%% Read and plot the data from Arduino
Ts = 0.01; % Sampling time (s)
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

