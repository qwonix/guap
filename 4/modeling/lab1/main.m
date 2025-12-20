%% Генерация выборок и вывод графиков в формате 3x3
clear; close all; clc;

% Параметры
N_values = [1000, 5000, 10000];
num_sets = length(N_values);

% Параметры генераторов
% Мультипликативный конгруэнтный
A = 7^5;
C = 0;
M = 2^32 - 1;
p = 52;
R0 = 2^(-p);

% Фибоначчи
a = 63;
b = 31;

% Подготовка контейнеров
multiplicative = cell(1, num_sets);
fibonacci = cell(1, num_sets);
mersenne = cell(1, num_sets);

% Генерация выборок
for i = 1:num_sets
    N = N_values(i);

    %% 1) Мультипликативный конгруэнтный
    R = zeros(1, N);
    R(1) = R0;
    for n = 2:N
        R(n) = mod(A * R(n-1) + C, M);
    end
    multiplicative{i} = R / M;

    %% 2) Фибоначчи с запаздыванием
    % Инициализируем начальными значениями в (0,1)
    init_len = max(a, b);
    init_values = rand(1, init_len);
    Rf = zeros(1, N);
    % если N < init_len — используем только первые N начальных значений
    if N <= init_len
        Rf(1:N) = init_values(1:N);
    else
        Rf(1:init_len) = init_values;
        for n = init_len+1:N
            if Rf(n-a) >= Rf(n-b)
                Rf(n) = Rf(n-a) - Rf(n-b);
            else
                Rf(n) = Rf(n-a) - Rf(n-b) + 1;
            end
        end
    end
    fibonacci{i} = Rf;

    %% 3) Вихрь Мерсенна
    s = RandStream('mt19937ar');
    mersenne{i} = rand(s, 1, N);
end

titles = {'Мультипликативный', 'Фибоначчи', 'Мерсенна'};

%% -------- ГИСТОГРАММЫ (3 × 3) --------
figure('Position',[100 100 1200 800],'Name','Гистограммы');
for algo = 1:3
    for i = 1:num_sets
        subplot(3, num_sets, (algo-1)*num_sets + i);

        switch algo
            case 1, data = multiplicative{i};
            case 2, data = fibonacci{i};
            case 3, data = mersenne{i};
        end

        histogram(data, 20, 'Normalization', 'pdf');
        title(sprintf('%s, N=%d', titles{algo}, N_values(i)));
        xlabel('Значение'); ylabel('Вероятность');
        xlim([0 1]); grid on;
    end
end

%% -------- ЭМПИРИЧЕСКИЕ ФУНКЦИИ РАСПРЕДЕЛЕНИЯ (3 × 3) --------
figure('Position',[100 100 1200 800],'Name','Эмпирические функции распределения');
for algo = 1:3
    for i = 1:num_sets
        subplot(3, num_sets, (algo-1)*num_sets + i);

        switch algo
            case 1, data = multiplicative{i};
            case 2, data = fibonacci{i};
            case 3, data = mersenne{i};
        end

        sorted_data = sort(data(:));
        n = length(sorted_data);
        F_emp = (1:n)'/n;

        % stairs с добавлением крайних точек 0 и 1
        stairs([0; sorted_data; 1], [0; F_emp; 1], 'b', 'LineWidth', 1.5);
        hold on;
        plot([0 1], [0 1], 'r--'); % кривая F(x)=x (идеал для U(0,1))
        hold off;

        title(sprintf('%s, N=%d', titles{algo}, N_values(i)));
        xlabel('x'); ylabel('F(x)');
        xlim([0 1]); ylim([0 1]); grid on;
    end
end

%% -------- ТЕСТ «РАСПРЕДЕЛЕНИЕ НА ПЛОСКОСТИ» (3 × 3) --------
figure('Position',[100 100 1200 800],'Name','Распределение на плоскости');
for algo = 1:3
    for i = 1:num_sets
        subplot(3, num_sets, (algo-1)*num_sets + i);

        switch algo
            case 1, data = multiplicative{i};
            case 2, data = fibonacci{i};
            case 3, data = mersenne{i};
        end

        % Рисуем R_n vs R_{n+1}
        plot(data(1:end-1), data(2:end), '.', 'MarkerSize', 4);
        title(sprintf('%s, N=%d', titles{algo}, N_values(i)));
        xlabel('R_n'); ylabel('R_{n+1}');
        axis([0 1 0 1]); axis square; grid on;
    end
end

%% -------- СТАТИСТИКИ (мат. ожидание, дисперсия, СКО) --------
for algo = 1:3
    for i = 1:num_sets
        switch algo
            case 1, data = multiplicative{i};
            case 2, data = fibonacci{i};
            case 3, data = mersenne{i};
        end

        m = mean(data);
        v = var(data);
        s = std(data);

        fprintf('%s, N=%d:\n', titles{algo}, N_values(i));
        fprintf('  Мат. ожидание = %.6f\n', m);
        fprintf('  Дисперсия     = %.6f\n', v);
        fprintf('  СКО           = %.6f\n\n', s);
    end
end

%% -------- Теоретическое U(0,1) --------
mean_theory = 0.5;
var_theory = 1/12;
std_theory = sqrt(1/12);

fprintf('Теоретические значения U(0,1):\n');
fprintf('  Мат. ожидание = %.6f\n', mean_theory);
fprintf('  Дисперсия     = %.6f\n', var_theory);
fprintf('  СКО           = %.6f\n\n', std_theory);

x = linspace(-0.5, 1.5, 1000);
f = zeros(size(x));
f(x>=0 & x<=1) = 1;

F = zeros(size(x));
F(x>=0 & x<=1) = x(x>=0 & x<=1);
F(x>1) = 1;

figure('Position',[100 100 1200 500],'Name','U(0,1) теория');
subplot(1,2,1);
plot(x, f, 'b', 'LineWidth', 2);
title('Плотность f(x)');
xlabel('x'); ylabel('f(x)'); grid on;

subplot(1,2,2);
plot(x, F, 'b', 'LineWidth', 2);
title('Функция распределения F(x)');
xlabel('x'); ylabel('F(x)'); grid on;
