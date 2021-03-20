%% frequency to theoretical rpm
clear all
freq = 100
pulses_per_rev = 4000
freqclock = 600e6

dt = 1/freq
rps = freq/pulses_per_rev;
rpm = rps*60;

%% rpm to theoretical frequency
clear all 
rpm = 1.5
pulses_per_rev = 4000
freqclock = 600e6

rps = rpm/60
freq = rps*pulses_per_rev