clc; clear; close all;

amount = [1000, 5000, 10000];
lambda = 1.5;

% Параметры прямоугольника
a = 0;
b = 6 / lambda;
M = lambda;

% Функция плотности
f = @(x) lambda * exp(-lambda * x);

% Окна для графиков
figure_hist = figure('Name','Гистограммы');
figure_ecdf = figure('Name','Эмпирические функции распределения');
figure_scatter = figure('Name','Распределение на плоскости');

for k = 1:length(amount)
    N = amount(k);

    y = zeros(1, N);
    i = 1;

    % Метод исключения
    while i <= N
        x = a + (b - a) * rand;
        z = M * rand;
        if z <= f(x)
            y(i) = x;
            i = i + 1;
        end
    end

    % Статистические оценки
    disp("N = " + N);
    disp("Мат. ожидание = " + mean(y));
    disp("Дисперсия     = " + var(y));
    disp("СКО           = " + std(y));
    disp("------------------------------------------");

    % ---------- ГИСТОГРАММЫ ----------
    figure(figure_hist);
    subplot(3,1,k);
    histogram(y, 30, 'Normalization', 'pdf');
    grid on;
    title(['Гистограмма, N = ', num2str(N)]);
    xlabel('x');
    ylabel('f(x)');

    % ---------- ЭМПИРИЧЕСКАЯ ФР ----------
    figure(figure_ecdf);
    subplot(3,1,k);
    [F, z_ecdf] = ecdf(y);
    plot(z_ecdf, F, 'LineWidth', 1);
    grid on;
    title(['ЭФР, N = ', num2str(N)]);
    xlabel('x');
    ylabel('F(x)');

    % ---------- РАСПРЕДЕЛЕНИЕ НА ПЛОСКОСТИ ----------
    figure(figure_scatter);
    subplot(3,1,k);
    scatter(y(1:2:end-1), y(2:2:end), '.');
    axis equal;
    grid on;
    title(['Распределение на плоскости, N = ', num2str(N)]);
    xlabel('R_i');
    ylabel('R_{i+1}');
end
