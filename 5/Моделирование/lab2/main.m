clc; clear; close all;

% Параметры варианта
m = 7;
sigma = 1;

% Воспроизводимость
rng(0);

% Объёмы выборок
N_values = [1000, 5000, 10000];
numSets = numel(N_values);

% Список методов (4 метода -> 12 наборов)
methods = {'CLT','Box-Muller-trig','Marsaglia-polar','randn'};
numMethods = numel(methods);

% x для теоретических кривых
x = linspace(m-4*sigma, m+4*sigma, 1000);

% Теоретическая плотность и функция распределения
pdf_theory = (1/(sigma*sqrt(2*pi))) .* exp(-((x-m).^2)/(2*sigma^2));
cdf_theory = 0.5 .* (1 + erf((x-m)/(sigma*sqrt(2))));

% Ячейка для хранения всех наборов: data{methodIndex, setIndex}
data = cell(numMethods, numSets);

% Генерация данных
for im = 1:numMethods
    methodName = methods{im};
    for k = 1:numSets
        N = N_values(k);
        switch methodName
            case 'CLT'
                % Сумма 12 равномерных по строкам -> стандартная нормаль прибл.
                X = sum(rand(N,12), 2) - 6;
                X = m + sigma .* X;
            case 'Box-Muller-trig'
                % Классический тригонометрический Box-Muller (по парам r1,r2)
                r1 = rand(N,1);
                r2 = rand(N,1);
                z = sqrt(-2 .* log(r2)) .* cos(2*pi*r1); % даёт N нормалей
                X = m + sigma .* z;
            case 'Marsaglia-polar'
                % Полярный метод Марсалья (векторизованный батчами)
                Z = zeros(N,1);
                idx = 0;
                while idx < N
                    need = N - idx;
                    batch = max(ceil(need*1.3), 100); % чуть больше для фильтрации
                    u1 = 2*rand(batch,1) - 1;
                    u2 = 2*rand(batch,1) - 1;
                    s = u1.^2 + u2.^2;
                    ok = find(s>0 & s<1);
                    take = min(numel(ok), need);
                    if take > 0
                        sel = ok(1:take);
                        factor = sqrt(-2 .* log(s(sel)) ./ s(sel));
                        % используем u1(sel) * factor как первую последовательность
                        Z(idx+1:idx+take) = u1(sel) .* factor;
                        idx = idx + take;
                    end
                end
                X = m + sigma .* Z;
            case 'randn'
                % Встроенный генератор
                X = m + sigma .* randn(N,1);
            otherwise
                error('Неизвестный метод: %s', methodName);
        end
        data{im,k} = X; %#ok<SAGROW>
    end
end

% Имена для вывода
names = methods;

% --- Построение гистограмм (4 x 3) ---
figure('Name','Гистограммы (4 метода x 3 объёма)','Position',[100 100 1200 900]);
for im = 1:numMethods
    for k = 1:numSets
        subplot(numMethods, numSets, (im-1)*numSets + k);
        d = data{im,k};
        histogram(d, 30, 'Normalization', 'pdf');
        hold on;
        plot(x, pdf_theory, 'r', 'LineWidth', 1.5);
        hold off;
        title(sprintf('%s, N=%d', names{im}, N_values(k)));
        xlabel('x'); ylabel('f(x)');
        grid on;
    end
end

% --- Эмпирические функции распределения (ECDF) ---
figure('Name','Эмпирические функции распределения','Position',[100 100 1200 900]);
for im = 1:numMethods
    for k = 1:numSets
        subplot(numMethods, numSets, (im-1)*numSets + k);
        d = data{im,k};
        [f,xx] = ecdf(d);
        stairs(xx,f,'b','LineWidth',1.2);
        hold on;
        plot(x, cdf_theory, 'r--', 'LineWidth', 1.2);
        hold off;
        title(sprintf('%s, N=%d', names{im}, N_values(k)));
        xlabel('x'); ylabel('F(x)');
        grid on;
    end
end

% --- Распределение на плоскости x_n vs x_{n+1} ---
figure('Name','Распределение на плоскости','Position',[100 100 1200 900]);
for im = 1:numMethods
    for k = 1:numSets
        subplot(numMethods, numSets, (im-1)*numSets + k);
        d = data{im,k};
        plot(d(1:end-1), d(2:end), '.', 'MarkerSize', 4);
        xlabel('x_n'); ylabel('x_{n+1}');
        title(sprintf('%s, N=%d', names{im}, N_values(k)));
        grid on;
    end
end

% --- QQ-плоты (ручная реализация, без toolbox) ---
figure('Name','QQ-plots (ручные)','Position',[100 100 1200 900]);
for im = 1:numMethods
    for k = 1:numSets
        subplot(numMethods, numSets, (im-1)*numSets + k);
        d = data{im,k};
        d_sorted = sort(d);
        N = numel(d_sorted);
        p = ((1:N) - 0.5) ./ N;
        % теоретические квантили
        q_theory = m + sigma .* sqrt(2) .* erfinv(2*p - 1);
        plot(q_theory, d_sorted, '.');
        hold on;
        mn = min([q_theory(:); d_sorted(:)]);
        mx = max([q_theory(:); d_sorted(:)]);
        plot([mn mx], [mn mx], 'r--');
        hold off;
        xlabel('Теор. квантили'); ylabel('Выборочные квантили');
        title(sprintf('QQ: %s, N=%d', names{im}, N_values(k)));
        grid on;
    end
end

% --- Вывод статистик (таблица) ---
fprintf('\nОценки статистик (m=%.2f, sigma=%.2f)\n', m, sigma);
fprintf('Теория: mean = %.2f, var = %.2f, std = %.2f\n\n', m, sigma^2, sigma);

% Создадим матрицу для хранения результатов (numMethods x numSets x 3)
stats = zeros(numMethods, numSets, 3); % [mean, var, std]

for im = 1:numMethods
    for k = 1:numSets
        d = data{im,k};
        mu_hat = mean(d);
        var_hat = var(d);
        std_hat = std(d);
        stats(im,k,1) = mu_hat;
        stats(im,k,2) = var_hat;
        stats(im,k,3) = std_hat;
        fprintf('%s, N=%d:\n', names{im}, N_values(k));
        fprintf(' mean = %.6f\n', mu_hat);
        fprintf(' var  = %.6f\n', var_hat);
        fprintf(' std  = %.6f\n\n', std_hat);
    end
end

% (Опционально) вывести таблицу в компактном виде
fprintf('Краткая таблица оценок (строки - методы, столбцы - N=1000,5000,10000):\n');
for im = 1:numMethods
    fprintf('%-18s |', names{im});
    for k = 1:numSets
        fprintf(' mean=%.3f std=%.3f |', stats(im,k,1), stats(im,k,3));
    end
    fprintf('\n');
end
