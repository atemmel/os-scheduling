format long;

fifo = importdata("fifo.csv");
rr   = importdata("rr.csv");

mu_fifo = mean(fifo);
mu_rr   = mean(rr);

sigma_fifo = std(fifo);
sigma_rr   = std(rr);

err_fifo = sigma_fifo / sqrt(size(fifo,1));
err_rr   = sigma_rr / sqrt(size(rr,1));

pd_fifo = fitdist(fifo, 'Normal');
pd_rr   = fitdist(rr, 'Normal');
ci_fifo = paramci(pd_fifo,'Alpha', 0.1);
ci_rr   = paramci(pd_rr, 'Alpha', 0.1);

x_fifo = 4500:6000;
y_fifo = pdf(pd_fifo, x_fifo);
x_rr = 6000:7600;
y_rr = pdf(pd_rr, x_rr);

figure(1)
hold on
p_fifo = histogram(fifo, 100);
line(x_fifo, y_fifo * 8000);

p_rr = histogram(rr, 100);
line(x_rr, y_rr * 14000);

xline(ci_fifo(1, 1), '-m');
xline(ci_fifo(2, 1), '-m');

xline(ci_rr(1, 1), '-m');
xline(ci_rr(2, 1), '-m')

%{
xline(prctile(fifo, 5), '-m', '5%');
xline(prctile(fifo, 95), '-m', '95%');

xline(prctile(rr, 5), '-m', '5%');
xline(prctile(rr, 95), '-m', '95%');
%}
hold off

legend([p_fifo, p_rr], {'Fifo', 'Round Robin'})

figure(2)
tiledlayout(1,2)
nexttile
bar(diag([mu_fifo, mu_rr]), 'stacked')
legend({'µ Fifo', 'µ Round Robin'})
nexttile
bar(diag([sigma_fifo ; sigma_rr]), 'stacked')
legend({'std Fifo', 'std Round Robin'})