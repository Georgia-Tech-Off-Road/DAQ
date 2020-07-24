try
    fopen(s);
catch err
    fclose(instrfind);
    error('Make sure you select the correct COM Port where the Arduino is connected.');
end
pause(3);
t = uint8(5);
fwrite(s, t, 'uint8');
pause(0.5)
reading = fscanf(s);
disp(reading);
fclose(s);