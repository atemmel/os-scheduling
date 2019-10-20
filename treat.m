format long;

fifo = importdata("fifo.csv");
rr   = importdata("rr.csv");

mu_fifo = mean(fifo);
mu_rr   = mean(rr);

sigma_fifo = std(fifo);
sigma_rr   = std(rr);

err_fifo = sigma_fifo / sqrt(size(fifo,1));
err_rr   = sigma_rr / sqrt(size(rr,1));

pd_fifo = makedist('Normal', 'mu', mu_fifo, 'sigma', sigma_fifo);
pd_rr   = makedist('Normal', 'mu', mu_rr, 'sigma', sigma_rr);
ci_fifo = paramci(pd_fifo,'Alpha', .1);
ci_rr   = paramci(pd_rr, 'Alpha', .1);

x_fifo = 4500:6000;
y_fifo = pdf(pd_fifo, x_fifo);
x_rr = 6000:7600;
y_rr = pdf(pd_rr, x_rr);

figure
hold on
histogram(fifo, 'Normalization', 'pdf');
line(x_fifo, y_fifo);

histogram(rr, 'Normalization', 'pdf');
line(x_rr, y_rr);