dt = 1000e-6;
pulse_per_rev = 20;
ppr = pulse_per_rev;
% dt
ticks = dt_to_ticks(dt, ppr)
rpm = dt_to_rpm(dt, ppr)
freq = dt_to_freq(dt, ppr)


% % ticks
% ticks = 150e4
% dt = ticks_to_dt(ticks, ppr)
% freq = dt_to_freq(dt, ppr)
% rpm = freq_to_rpm(freq, ppr)
%% functions
% ticks to dt 
function dt = ticks_to_dt(elapsed_ticks, pulses_per_rev)
    freqclock = 600e6;
    dt = elapsed_ticks/freqclock;
    
end

% dt to ticks 
function ticks = dt_to_ticks(dt, pulses_per_rev)
    freqclock = 600e6;
    elapsed_ticks = dt*freqclock;
    ticks = elapsed_ticks;
end

% frequency to theoretical rpm
function rpm = freq_to_rpm(freq, pulses_per_rev)
    freqclock = 600e6;
    rps = freq/pulses_per_rev;
    rpm = rps*60;
end 

% rpm to theoretical frequency
function freq = rpm_to_freq(rpm, pulses_per_rev)
    freqclock = 600e6;
    rps = rpm/60;
    freq = rps*pulses_per_rev;
end

% dt to theoretical frequency
function freq = dt_to_freq(dt, pulses_per_rev)
    freq = 1/dt;
end

% dt to rpm
function rpm = dt_to_rpm(dt, pulses_per_rev)
    freq = 1/dt;
    rpm = freq_to_rpm(freq, pulses_per_rev);
end 