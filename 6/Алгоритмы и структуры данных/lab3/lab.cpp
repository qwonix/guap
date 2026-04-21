// =============================================================
//  Вариант 11 | Задача 3
//  Стек: динамический (связный список)
//  Очереди: статические (массив, кольцевой буфер)
//
//  Система: 1 процессор P, 3 очереди F0/F1/F2, 1 стек S.
//  Правило задачи 3: прерванная задача возвращается из стека
//  ТОЛЬКО когда ВСЕ три очереди пусты.
// =============================================================

#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// =============================================================
//  СТРУКТУРА «ЗАДАЧА»
// =============================================================
struct Task {
    std::string name;      // имя задачи (например "a", "b", ...)
    int priority;          // приоритет: 0 = высший, 1 = средний, 2 = низший
    int totalTime;         // полное время выполнения (в тактах)
    int remaining;         // оставшееся время выполнения
    int arrival;           // такт поступления в систему
};

// =============================================================
//  СТАТИЧЕСКАЯ ОЧЕРЕДЬ
//  Реализована через массив фиксированного размера (кольцевой буфер).
//  head — откуда берём, tail — куда добавляем.
// =============================================================
const int QUEUE_CAP = 50; // максимальная ёмкость одной очереди

struct StaticQueue {
    Task* buf[QUEUE_CAP]; // массив указателей на задачи
    int   head;           // индекс первого элемента
    int   tail;           // индекс за последним элементом
    int   sz;             // текущее количество элементов
};

// Инициализация очереди
void initQueue(StaticQueue& q) {
    q.head = 0;
    q.tail = 0;
    q.sz   = 0;
}

// Проверки
bool qEmpty(const StaticQueue& q) { return q.sz == 0; }
bool qFull (const StaticQueue& q) { return q.sz >= QUEUE_CAP; }

// Добавить задачу в конец очереди (enqueue)
bool enqueue(StaticQueue& q, Task* t) {
    if (qFull(q)) {
        std::cout << "[OVERFLOW] Queue is full! Task '"
                  << t->name << "' rejected.\n";
        return false;
    }
    q.buf[q.tail] = t;
    q.tail = (q.tail + 1) % QUEUE_CAP; // кольцевой сдвиг
    q.sz++;
    return true;
}

// Извлечь задачу из начала очереди (dequeue)
Task* dequeue(StaticQueue& q) {
    if (qEmpty(q)) return nullptr;
    Task* t = q.buf[q.head];
    q.head  = (q.head + 1) % QUEUE_CAP;
    q.sz--;
    return t;
}

// Вывод содержимого очереди: начало слева, конец справа
void printQueue(const StaticQueue& q) {
    for (int i = 0; i < q.sz; i++) {
        std::cout << q.buf[(q.head + i) % QUEUE_CAP]->name;
        if (i < q.sz - 1) std::cout << " ";
    }
}

// =============================================================
//  ДИНАМИЧЕСКИЙ СТЕК
//  Реализован через односвязный список.
//  Каждый новый узел выделяется в памяти через new.
// =============================================================
struct StackNode {
    Task*      task; // хранимая задача
    StackNode* next; // указатель на узел ниже в стеке
};

struct DynStack {
    StackNode* top; // вершина стека (nullptr если стек пуст)
    int        sz;  // количество элементов
};

// Инициализация стека
void initStack(DynStack& s) {
    s.top = nullptr;
    s.sz  = 0;
}

// Проверка на пустоту
bool sEmpty(const DynStack& s) { return s.top == nullptr; }

// Добавить задачу на вершину стека (push)
void push(DynStack& s, Task* t) {
    StackNode* node = new StackNode(); // динамическое выделение памяти
    node->task = t;
    node->next = s.top; // новый узел смотрит на старую вершину
    s.top      = node;  // новый узел становится вершиной
    s.sz++;
}

// Извлечь задачу с вершины стека (pop)
Task* pop(DynStack& s) {
    if (sEmpty(s)) return nullptr;
    StackNode* node = s.top;
    Task*      t    = node->task;
    s.top           = node->next;
    delete node; // освобождаем память узла
    s.sz--;
    return t;
}

// Вывод содержимого стека: вершина слева, дно справа
void printStack(const DynStack& s) {
    StackNode* cur = s.top;
    bool first = true;
    while (cur != nullptr) {
        if (!first) std::cout << " ";
        std::cout << cur->task->name;
        first = false;
        cur   = cur->next;
    }
}

// Полная очистка стека (освобождение памяти)
void freeStack(DynStack& s) {
    while (!sEmpty(s)) {
        Task* t = pop(s);
        delete t;
    }
}

// =============================================================
//  СИСТЕМА
// =============================================================
struct System {
    StaticQueue Q[3]; // Q[0]=F0 (высший приоритет), Q[1]=F1, Q[2]=F2
    DynStack    S;    // стек S
    Task*       cpu;  // задача в процессоре (nullptr = свободен)
    int         time; // текущий такт
};

// Инициализация системы
void initSys(System& sys) {
    for (int i = 0; i < 3; i++) initQueue(sys.Q[i]);
    initStack(sys.S);
    sys.cpu  = nullptr;
    sys.time = 0;
}

// Проверка: все три очереди пусты?
bool allEmpty(const System& sys) {
    for (int i = 0; i < 3; i++)
        if (!qEmpty(sys.Q[i])) return false;
    return true;
}

// Наивысший приоритет среди задач в очередях.
// Возвращает 0, 1 или 2. Возвращает -1 если все очереди пусты.
int bestPrio(const System& sys) {
    for (int p = 0; p <= 2; p++)
        if (!qEmpty(sys.Q[p])) return p;
    return -1;
}

// Взять задачу из очереди с наивысшим приоритетом
Task* takeFromQueues(System& sys) {
    for (int p = 0; p <= 2; p++)
        if (!qEmpty(sys.Q[p])) return dequeue(sys.Q[p]);
    return nullptr;
}

// Вывод состояния системы (на текущем такте)
void printState(const System& sys) {
    std::cout << "Timer: " << sys.time;
    std::cout << "  F0:["; printQueue(sys.Q[0]); std::cout << "]";
    std::cout << "  F1:["; printQueue(sys.Q[1]); std::cout << "]";
    std::cout << "  F2:["; printQueue(sys.Q[2]); std::cout << "]";
    std::cout << "  CPU:";
    if (sys.cpu) std::cout << sys.cpu->name;
    else         std::cout << "-";
    std::cout << "  Stack:["; printStack(sys.S); std::cout << "]\n";
}

// =============================================================
//  ОДИН ТАКТ РАБОТЫ СИСТЕМЫ
//
//  Порядок событий внутри такта:
//    1. Добавить поступившие задачи в очереди
//    2. Проверить, нужно ли прервать текущую задачу
//    3. Загрузить процессор (если свободен)
//    4. Вывести состояние (ПЕРЕД выполнением — так соответствует примеру)
//    5. Выполнить один такт: уменьшить remaining
//       Если задача завершена — удалить её (следующая загрузится в след. такте)
// =============================================================
void tick(System& sys, std::vector<Task*>& arrivals) {

    // --- 1. Добавить поступившие задачи в очереди ---
    for (Task* t : arrivals) {
        // задача идёт в очередь, соответствующую её приоритету
        enqueue(sys.Q[t->priority], t);
    }

    // --- 2. Проверить прерывание текущей задачи ---
    // Условие: в очереди есть задача с БОЛЕЕ высоким приоритетом (меньший номер)
    if (sys.cpu != nullptr) {
        int bp = bestPrio(sys);
        if (bp != -1 && bp < sys.cpu->priority) {
            // Текущую задачу кладём в стек и освобождаем процессор
            std::cout << "  [PREEMPT] Task '" << sys.cpu->name
                      << "' (p=" << sys.cpu->priority
                      << ") preempted by priority " << bp << " task.\n";
            push(sys.S, sys.cpu);
            sys.cpu = nullptr;
        }
    }

    // --- 3. Загрузить процессор если свободен ---
    if (sys.cpu == nullptr) {
        if (!allEmpty(sys)) {
            // Есть задачи в очередях — берём наиболее приоритетную
            sys.cpu = takeFromQueues(sys);
        } else if (!sEmpty(sys.S)) {
            // ТОЛЬКО если все очереди пусты — берём из стека
            // (ключевое правило задачи 3)
            sys.cpu = pop(sys.S);
        }
    }

    // --- 4. Вывод состояния (до выполнения такта) ---
    printState(sys);
    sys.time++;

    // --- 5. Выполнить один такт обработки ---
    if (sys.cpu != nullptr) {
        sys.cpu->remaining--;
        if (sys.cpu->remaining <= 0) {
            // Задача завершена
            std::cout << "          >> Task '" << sys.cpu->name
                      << "' completed.\n";
            delete sys.cpu; // освобождаем память задачи
            sys.cpu = nullptr;
            // Следующая задача будет загружена в начале СЛЕДУЮЩЕГО такта
        }
    }
}

// =============================================================
//  ЗАПИСЬ В РАСПИСАНИИ (такт прихода + задача)
// =============================================================
struct Entry {
    int   when; // такт поступления
    Task* task; // указатель на задачу
};

// =============================================================
//  ЗАПУСК СИМУЛЯЦИИ ПО РАСПИСАНИЮ
// =============================================================
void simulate(System& sys, std::vector<Entry>& sched) {
    // Подсчитываем верхнюю границу времени симуляции
    int latest = 0, totalWork = 0;
    for (auto& e : sched) {
        latest    = std::max(latest, e.when);
        totalWork += e.task->totalTime;
    }
    int endT = latest + totalWork + 5; // с небольшим запасом

    for (int t = 0; t < endT; t++) {
        // Собрать задачи, поступающие именно на такт t
        std::vector<Task*> arr;
        for (auto& e : sched) {
            if (e.when == t) arr.push_back(e.task);
        }

        // Вывод сообщений о поступлении
        for (Task* tk : arr) {
            std::cout << "  --> Task '" << tk->name
                      << "' arrives  priority=" << tk->priority
                      << "  duration=" << tk->totalTime << "\n";
        }

        // Выполнить такт
        tick(sys, arr);

        // Ранний выход: нет будущих задач и система простаивает
        bool future = false;
        for (auto& e : sched) {
            if (e.when > t) { future = true; break; }
        }
        if (!future && sys.cpu == nullptr && allEmpty(sys) && sEmpty(sys.S))
            break;
    }

    std::cout << "\n=== Simulation complete. All tasks done. ===\n";
}

// =============================================================
//  РУЧНОЕ ДОБАВЛЕНИЕ ЗАДАЧИ
// =============================================================
void addManual(std::vector<Entry>& sched, int& counter) {
    Task* t = new Task();
    std::cout << "  Task name        : "; std::cin >> t->name;
    std::cout << "  Priority (0/1/2) : "; std::cin >> t->priority;
    std::cout << "  Duration (ticks) : "; std::cin >> t->totalTime;
    std::cout << "  Arrival time     : "; std::cin >> t->arrival;

    // Валидация
    if (t->priority < 0 || t->priority > 2) {
        std::cout << "  [ERROR] Priority must be 0, 1 or 2. Task rejected.\n";
        delete t; return;
    }
    if (t->totalTime <= 0) {
        std::cout << "  [ERROR] Duration must be > 0. Task rejected.\n";
        delete t; return;
    }
    if (t->arrival < 0) {
        std::cout << "  [ERROR] Arrival time must be >= 0. Task rejected.\n";
        delete t; return;
    }

    t->remaining = t->totalTime;
    sched.push_back({t->arrival, t});
    counter++;
    std::cout << "  Task '" << t->name << "' added successfully.\n";
}

// =============================================================
//  АВТОГЕНЕРАЦИЯ ЗАДАЧ
// =============================================================
void autoGen(std::vector<Entry>& sched, int& counter) {
    int n;
    std::cout << "  How many tasks to generate? ";
    std::cin >> n;
    srand(static_cast<unsigned>(time(nullptr)));

    for (int i = 0; i < n; i++) {
        Task* t      = new Task();
        // Имена: A, B, C, ... (циклически)
        t->name      = std::string(1, char('A' + (counter % 26)));
        t->priority  = rand() % 3;       // 0, 1 или 2
        t->totalTime = 1 + rand() % 8;   // от 1 до 8 тактов
        t->remaining = t->totalTime;
        t->arrival   = rand() % 15;      // от такта 0 до 14

        sched.push_back({t->arrival, t});
        counter++;

        std::cout << "  Generated: " << t->name
                  << "  priority=" << t->priority
                  << "  duration=" << t->totalTime
                  << "  arrival="  << t->arrival << "\n";
    }
}

// =============================================================
//  ВСТРОЕННЫЙ ПРИМЕР (адаптирован из методички)
// =============================================================
void builtinExample(std::vector<Entry>& sched) {
    // Вспомогательная лямбда для краткости
    auto add = [&](const std::string& nm, int prio, int dur, int arr) {
        Task* t = new Task{nm, prio, dur, dur, arr};
        sched.push_back({arr, t});
    };
    //    имя  приор  длит  приход
    add("a",   2,    10,    2);
    add("b",   1,     5,    5);
    add("c",   0,     3,    7);
    add("d",   0,     5,   22);
    add("e",   1,     3,   22);
    add("f",   2,     5,   25);
}

// =============================================================
//  ГЛАВНАЯ ФУНКЦИЯ
// =============================================================
int main() {
    std::cout << "=================================================\n";
    std::cout << " Variant 11 | Task 3\n";
    std::cout << " Stack: DYNAMIC  |  Queues: STATIC\n";
    std::cout << " Rule: stack task resumes ONLY when all queues empty\n";
    std::cout << "=================================================\n\n";

    int choice;
    std::cout << "Select mode:\n";
    std::cout << "  1 - Built-in example (from the manual)\n";
    std::cout << "  2 - Enter tasks manually\n";
    std::cout << "  3 - Auto-generate tasks\n";
    std::cout << "Choice: ";
    std::cin >> choice;

    std::vector<Entry> sched;
    int counter = 0;

    if (choice == 1) {
        builtinExample(sched);
        std::cout << "\nRunning built-in example...\n\n";
    }
    else if (choice == 2) {
        int n;
        std::cout << "How many tasks? ";
        std::cin >> n;
        for (int i = 0; i < n; i++) {
            std::cout << "\n--- Task " << (i + 1) << " ---\n";
            addManual(sched, counter);
        }
    }
    else if (choice == 3) {
        autoGen(sched, counter);
    }
    else {
        std::cout << "Invalid choice.\n";
        return 1;
    }

    // Создаём и запускаем систему
    System sys;
    initSys(sys);

    std::cout << "\n--- Simulation start ---\n\n";
    simulate(sys, sched);

    // Освобождаем память стека (если осталось что-то незавершённое)
    freeStack(sys.S);

    return 0;
}
