clc; clear; close all;

amount = [1000, 5000, 10000];
lambda = 1.5;

% Теоретические значения
theor_mean = 1 / lambda;
theor_var  = 1 / lambda^2;
theor_std  = 1 / lambda;

disp("Теоретические значения:");
disp("Мат. ожидание = " + theor_mean);
disp("Дисперсия     = " + theor_var);
disp("СКО           = " + theor_std);
disp("==========================================");

% Окна для графиков
figure_hist = figure('Name','Гистограммы');
figure_ecdf = figure('Name','Эмпирические функции распределения');
figure_scatter = figure('Name','Распределение на плоскости');

for n = 1:length(amount)
    N = amount(n);

    % Генерация равномерных ПСЧ
    gamma = rand(1, N);

    % Метод обратной функции
    y = -log(1 - gamma) / lambda;

    % Оценки характеристик
    disp("N = " + N);
    disp("Мат. ожидание (эмп.) = " + mean(y));
    disp("Дисперсия (эмп.)     = " + var(y));
    disp("СКО (эмп.)           = " + std(y));
    disp("------------------------------------------");

    % ---------- ГИСТОГРАММЫ ----------
    figure(figure_hist);
    subplot(3,1,n);
    histogram(y, 30, 'Normalization', 'pdf');
    grid on;
    title(['N = ', num2str(N)]);
    xlabel('x');
    ylabel('f(x)');

    % ---------- ЭМПИРИЧЕСКАЯ ФР ----------
    figure(figure_ecdf);
    subplot(3,1,n);
    ecdf(y);
    grid on;
    title(['N = ', num2str(N)]);
    xlabel('x');
    ylabel('F(x)');

    % ---------- РАСПРЕДЕЛЕНИЕ НА ПЛОСКОСТИ ----------
    figure(figure_scatter);
    subplot(3,1,n);
    scatter(y(1:2:end-1), y(2:2:end), '.');
    grid on;
    title(['N = ', num2str(N)]);
    xlabel('X_i');
    ylabel('X_{i+1}');
end
