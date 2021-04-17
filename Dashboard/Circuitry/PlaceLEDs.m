dim = 3271;
half = dim/2;

n = 24;
num = 100000;
 
anglebounds = [(pi + deg2rad(27.8)), -deg2rad(27.8)];
angle = [anglebounds(1) : (anglebounds(2) - anglebounds(1))/(n-1) : anglebounds(2), -pi/2];

z = (half) * exp(j*angle);
pos = [real(z); imag(z)];

posr = round(pos/5) * 5;


g = figure(1);
clf;
hold on
set(g, 'Position', [100,100,800,800]);
scatter(posr(1,:), posr(2,:), 150, [0,0,0], 'filled');

ang = 0 : 1/100 : 2*pi;
circle = (half - 100) * exp(j*ang);
plot(real(circle) + half*ones(1,length(circle)), imag(circle) + half*ones(1,length(circle)));


