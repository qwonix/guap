// ============================================================
// Лабораторная работа №4: Хеш-таблица
// Вариант 11: формат ключа БццццБ, 3000 сегментов,
//             метод разрешения коллизий — квадратичное опробование
//
// Формат ключа:
//   Позиция 0   : большая буква латиницы (A-Z)
//   Позиции 1-4 : цифры (0-9)
//   Позиция 5   : большая буква латиницы (A-Z)
//   Пример: A1234B, Z9876K
// ============================================================

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <ctime>

using namespace std;

// ============================================================
//  Константы
// ============================================================
const int NUM_SEGMENTS = 3000; // Количество сегментов хеш-таблицы
const int KEY_LENGTH   = 6;    // Длина ключа (всегда 6 символов)

// ============================================================
//  Состояния ячейки хеш-таблицы
// ============================================================
enum CellState {
    EMPTY,    // Ячейка пустая — никогда не занималась
    OCCUPIED, // Ячейка занята — хранит ключ
    DELETED   // Ячейка помечена как удалённая (tombstone)
              // При поиске пропускаем, при вставке — используем
};

// ============================================================
//  Структура одной ячейки хеш-таблицы
// ============================================================
struct HashCell {
    char key[KEY_LENGTH + 1]; // Ключ: 6 символов + нуль-терминатор '\0'
    CellState state;          // Текущее состояние ячейки

    // Конструктор: инициализируем пустой ячейкой
    HashCell() {
        memset(key, 0, sizeof(key));
        state = EMPTY;
    }
};

// ============================================================
//  Глобальные данные
// ============================================================

// Сама хеш-таблица
HashCell hashTable[NUM_SEGMENTS];

// Счётчик обращений к каждому сегменту (для гистограммы)
int hitCount[NUM_SEGMENTS] = {0};

// ============================================================
//  Хеш-функция
//
//  Используем полиномиальное хеширование (метод Горнера):
//    h = (key[0]*p^5 + key[1]*p^4 + ... + key[5]*p^0) % N
//  где p = 31 — простое число, N = 3000
//
//  Это обеспечивает хорошее равномерное распределение по сегментам,
//  потому что разные позиции символов дают разный вклад.
//  Вычисления ведём по модулю на каждом шаге, чтобы не было
//  переполнения (long long).
// ============================================================
int hashFunction(const char* key) {
    const long long p   = 31;          // Основание (простое число)
    const long long mod = NUM_SEGMENTS; // Модуль = количество сегментов
    long long hash  = 0;
    long long power = 1; // Текущая степень p: p^0, p^1, p^2 ...

    // Идём с конца строки, чтобы вычислить p^0 для key[5], p^1 для key[4] и т.д.
    for (int i = KEY_LENGTH - 1; i >= 0; i--) {
        hash  = (hash + (long long)key[i] * power % mod) % mod;
        power = power * p % mod;
    }

    return (int)hash; // Результат всегда в [0, NUM_SEGMENTS - 1]
}

// ============================================================
//  Проверка формата ключа
//
//  Возвращает true, если ключ соответствует формату БццццБ:
//    key[0] — большая буква A-Z
//    key[1..4] — цифры 0-9
//    key[5] — большая буква A-Z
// ============================================================
bool isValidKey(const char* key) {
    // Проверяем длину
    if ((int)strlen(key) != KEY_LENGTH)
        return false;

    // Позиция 0: должна быть заглавная латинская буква
    if (!isupper((unsigned char)key[0]))
        return false;

    // Позиции 1-4: должны быть цифры
    for (int i = 1; i <= 4; i++) {
        if (!isdigit((unsigned char)key[i]))
            return false;
    }

    // Позиция 5: должна быть заглавная латинская буква
    if (!isupper((unsigned char)key[5]))
        return false;

    return true;
}

// ============================================================
//  Квадратичное опробование
//
//  При коллизии следующий проверяемый индекс вычисляется как:
//    index(step) = (baseHash + step^2) % NUM_SEGMENTS
//
//  Шаг 0 → baseHash
//  Шаг 1 → baseHash + 1
//  Шаг 2 → baseHash + 4
//  Шаг 3 → baseHash + 9  и так далее
// ============================================================
int quadraticProbe(int baseHash, int step) {
    // Используем long long, чтобы step*step не переполнилось
    return (int)((baseHash + (long long)step * step) % NUM_SEGMENTS);
}

// ============================================================
//  Добавление ключа в хеш-таблицу
//
//  Алгоритм:
//    1. Проверяем формат ключа
//    2. Вычисляем базовый хэш
//    3. Квадратичным опробованием ищем свободную ячейку
//       (EMPTY или DELETED)
//    4. Вставляем ключ
//
//  Возвращает индекс вставки, или -1 при ошибке/переполнении
// ============================================================
int insertKey(const char* key) {
    // Шаг 1: проверка формата
    if (!isValidKey(key)) {
        cout << "Error: Invalid key format! Expected: Letter-Digit-Digit-Digit-Digit-Letter (e.g. A1234B)" << endl;
        return -1;
    }

    int base         = hashFunction(key); // Базовый адрес
    int insertAt     = -1;                // Куда будем вставлять
    int firstDeleted = -1;                // Первая tombstone-ячейка

    // Шаг 2: квадратичное опробование
    for (int step = 0; step < NUM_SEGMENTS; step++) {
        int probe = quadraticProbe(base, step);

        // Считаем обращение к этому сегменту (для гистограммы)
        hitCount[probe]++;

        if (hashTable[probe].state == EMPTY) {
            // Пустая ячейка — дальше элементов точно нет
            // Вставляем сюда (или в более раннюю tombstone, если была)
            insertAt = (firstDeleted != -1) ? firstDeleted : probe;
            break;
        } else if (hashTable[probe].state == DELETED) {
            // Tombstone — запоминаем первую, продолжаем поиск дубликата
            if (firstDeleted == -1)
                firstDeleted = probe;
        } else {
            // Ячейка занята — проверяем на дубликат
            if (strcmp(hashTable[probe].key, key) == 0) {
                cout << "Error: Key '" << key << "' already exists at segment " << probe << "!" << endl;
                return -1;
            }
        }
    }

    // Если не нашли EMPTY, но есть tombstone — используем её
    if (insertAt == -1 && firstDeleted != -1)
        insertAt = firstDeleted;

    // Шаг 3: проверка на переполнение
    if (insertAt == -1) {
        cout << "Error: Hash table is FULL! Cannot insert '" << key << "'." << endl;
        return -1;
    }

    // Шаг 4: вставка
    strncpy(hashTable[insertAt].key, key, KEY_LENGTH);
    hashTable[insertAt].key[KEY_LENGTH] = '\0';
    hashTable[insertAt].state = OCCUPIED;

    return insertAt;
}

// ============================================================
//  Поиск ключа по значению
//
//  Возвращает индекс найденного элемента, или -1 если не найден.
//  При квадратичном опробовании нельзя остановиться на DELETED —
//  нужно продолжать поиск. Остановка только на EMPTY.
// ============================================================
int searchByKey(const char* key) {
    if (!isValidKey(key)) {
        cout << "Error: Invalid key format!" << endl;
        return -1;
    }

    int base = hashFunction(key);

    for (int step = 0; step < NUM_SEGMENTS; step++) {
        int probe = quadraticProbe(base, step);

        if (hashTable[probe].state == EMPTY) {
            // Пустая ячейка — цепочка закончилась, элемента нет
            break;
        } else if (hashTable[probe].state == OCCUPIED) {
            if (strcmp(hashTable[probe].key, key) == 0)
                return probe; // Нашли!
        }
        // DELETED — пропускаем, продолжаем поиск по цепочке
    }

    return -1;
}

// ============================================================
//  Поиск по номеру сегмента
// ============================================================
void searchBySegment(int seg) {
    if (seg < 0 || seg >= NUM_SEGMENTS) {
        cout << "Error: Segment must be in range 0 to " << NUM_SEGMENTS - 1 << "!" << endl;
        return;
    }

    if (hashTable[seg].state == OCCUPIED) {
        cout << "Segment [" << seg << "]: key = " << hashTable[seg].key << endl;
    } else if (hashTable[seg].state == DELETED) {
        cout << "Segment [" << seg << "]: deleted (tombstone)." << endl;
    } else {
        cout << "Segment [" << seg << "]: empty." << endl;
    }
}

// ============================================================
//  Удаление ключа из хеш-таблицы
//
//  При квадратичном опробовании нельзя просто очистить ячейку
//  (иначе поиск других ключей сломается). Поэтому:
//    1. Помечаем ячейку как DELETED (tombstone)
//    2. Ищем и показываем элементы, которые вызвали коллизию
//       с удалённым (т.е. имеют тот же базовый хэш)
//
//  Tombstone-механизм обеспечивает корректность поиска:
//    — при поиске: пропускаем DELETED, останавливаемся на EMPTY
//    — при вставке: можно повторно использовать DELETED-ячейки
// ============================================================
bool deleteKey(const char* key) {
    if (!isValidKey(key)) {
        cout << "Error: Invalid key format!" << endl;
        return false;
    }

    int base         = hashFunction(key);
    int deletedIndex = -1;

    // Ищем элемент по цепочке квадратичного опробования
    for (int step = 0; step < NUM_SEGMENTS; step++) {
        int probe = quadraticProbe(base, step);
        if (hashTable[probe].state == EMPTY) break;
        if (hashTable[probe].state == OCCUPIED && strcmp(hashTable[probe].key, key) == 0) {
            deletedIndex = probe;
            break;
        }
    }

    if (deletedIndex == -1) {
        cout << "Key '" << key << "' not found in hash table!" << endl;
        return false;
    }

    // Помечаем как удалённый (tombstone)
    hashTable[deletedIndex].state = DELETED;
    memset(hashTable[deletedIndex].key, 0, KEY_LENGTH + 1);
    cout << "Key '" << key << "' deleted from segment " << deletedIndex << "." << endl;

    // ============================================================
    //  Поиск элементов, вызвавших коллизию с удалённым:
    //  Это элементы, чей базовый хэш совпадает с базовым хэшем
    //  удалённого ключа — они боролись за тот же исходный сегмент
    // ============================================================
    cout << "Searching for elements that caused collision (base hash = " << base << ")..." << endl;
    bool foundCollision = false;

    for (int i = 0; i < NUM_SEGMENTS; i++) {
        if (hashTable[i].state == OCCUPIED) {
            int elemBase = hashFunction(hashTable[i].key);
            if (elemBase == base) {
                // Этот элемент изначально метился в тот же сегмент
                cout << "  Colliding element: '" << hashTable[i].key
                     << "' at segment " << i
                     << " (base hash = " << elemBase << ")" << endl;
                foundCollision = true;
            }
        }
    }

    if (!foundCollision)
        cout << "  No colliding elements found." << endl;

    return true;
}

// ============================================================
//  Просмотр всей хеш-таблицы
// ============================================================
void printHashTable() {
    cout << "\n=== Hash Table Contents ===" << endl;
    int count = 0;
    for (int i = 0; i < NUM_SEGMENTS; i++) {
        if (hashTable[i].state == OCCUPIED) {
            cout << "  [" << i << "] " << hashTable[i].key << endl;
            count++;
        }
    }
    if (count == 0)
        cout << "  (empty)" << endl;
    else
        cout << "Total: " << count << " / " << NUM_SEGMENTS << " segments used." << endl;
}

// ============================================================
//  Экспорт данных в CSV-файл для построения гистограммы
//
//  Файл можно открыть в Excel (или LibreOffice Calc) и
//  построить диаграмму по столбцу "Hits".
// ============================================================
void exportHistogram(const char* filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot create file '" << filename << "'!" << endl;
        return;
    }

    // Заголовок CSV
    file << "Segment;Hits;Key" << endl;

    for (int i = 0; i < NUM_SEGMENTS; i++) {
        file << i << ";" << hitCount[i] << ";";
        if (hashTable[i].state == OCCUPIED)
            file << hashTable[i].key;
        file << endl;
    }

    file.close();
    cout << "Exported to '" << filename << "'. Open in Excel: Data -> From Text/CSV." << endl;
}

// ============================================================
//  Загрузка хеш-таблицы из CSV-файла (формат exportHistogram)
// ============================================================
void loadFromFile(const char* filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot open file '" << filename << "'!" << endl;
        return;
    }

    string line;
    getline(file, line); // Пропускаем строку-заголовок

    int count = 0;
    while (getline(file, line)) {
        // Формат: сегмент;количество_попаданий;ключ
        size_t sep1 = line.find(';');
        size_t sep2 = line.find(';', sep1 + 1);
        if (sep1 == string::npos || sep2 == string::npos) continue;

        string keyStr = line.substr(sep2 + 1);

        // Убираем возможный символ '\r' (Windows-формат строк)
        if (!keyStr.empty() && keyStr.back() == '\r')
            keyStr.pop_back();

        if ((int)keyStr.length() == KEY_LENGTH && isValidKey(keyStr.c_str())) {
            if (insertKey(keyStr.c_str()) >= 0)
                count++;
        }
    }

    file.close();
    cout << "Loaded " << count << " keys from '" << filename << "'." << endl;
}

// ============================================================
//  Генерация случайного ключа формата БццццБ
// ============================================================
void generateRandomKey(char* key) {
    key[0] = 'A' + rand() % 26;     // Случайная буква
    for (int i = 1; i <= 4; i++)
        key[i] = '0' + rand() % 10; // Случайная цифра
    key[5] = 'A' + rand() % 26;     // Случайная буква
    key[6] = '\0';
}

// ============================================================
//  Вывод меню
// ============================================================
void showMenu() {
    cout << "\n+--------------------------------------------------+" << endl;
    cout << "|  Hash Table | Key: L####L | N=3000 | Quad Probe |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << "|  1. Insert key                                   |" << endl;
    cout << "|  2. Search by key                                |" << endl;
    cout << "|  3. Search by segment number                     |" << endl;
    cout << "|  4. Delete key                                   |" << endl;
    cout << "|  5. View all entries                             |" << endl;
    cout << "|  6. Export histogram to CSV file                 |" << endl;
    cout << "|  7. Load from CSV file                           |" << endl;
    cout << "|  8. Generate random keys (for testing)           |" << endl;
    cout << "|  0. Exit                                         |" << endl;
    cout << "+--------------------------------------------------+" << endl;
    cout << "Choice: ";
}

// ============================================================
//  Главная функция
// ============================================================
int main() {
    // Инициализируем генератор случайных чисел текущим временем
    srand((unsigned int)time(NULL));

    int choice;
    char key[KEY_LENGTH + 10]; // Буфер для ввода ключа (чуть больше для защиты)

    cout << "Hash Table Program. Key format: Letter-Digit-Digit-Digit-Digit-Letter" << endl;
    cout << "Example valid keys: A1234B, Z9876K, M0000A" << endl;

    do {
        showMenu();

        // Защита от некорректного ввода (не числа)
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Please enter a number!" << endl;
            continue;
        }

        switch (choice) {

            // ---- Добавление ключа ----
            case 1: {
                cout << "Enter key (e.g. A1234B): ";
                cin >> key;
                int idx = insertKey(key);
                if (idx >= 0)
                    cout << "Inserted '" << key << "' at segment " << idx << "." << endl;
                break;
            }

            // ---- Поиск по ключу ----
            case 2: {
                cout << "Enter key to search: ";
                cin >> key;
                int idx = searchByKey(key);
                if (idx >= 0)
                    cout << "Key '" << key << "' found at segment " << idx << "." << endl;
                else
                    cout << "Key '" << key << "' not found." << endl;
                break;
            }

            // ---- Поиск по номеру сегмента ----
            case 3: {
                int seg;
                cout << "Enter segment number (0 - " << NUM_SEGMENTS - 1 << "): ";
                cin >> seg;
                searchBySegment(seg);
                break;
            }

            // ---- Удаление ключа ----
            case 4: {
                cout << "Enter key to delete: ";
                cin >> key;
                deleteKey(key);
                break;
            }

            // ---- Просмотр таблицы ----
            case 5: {
                printHashTable();
                break;
            }

            // ---- Экспорт гистограммы ----
            case 6: {
                char filename[256];
                cout << "Enter output filename (e.g. histogram.csv): ";
                cin >> filename;
                exportHistogram(filename);
                break;
            }

            // ---- Загрузка из файла ----
            case 7: {
                char filename[256];
                cout << "Enter filename to load from: ";
                cin >> filename;
                loadFromFile(filename);
                break;
            }

            // ---- Генерация случайных ключей ----
            case 8: {
                int n;
                cout << "Enter number of random keys to generate: ";
                cin >> n;
                if (n <= 0) { cout << "Error: Count must be positive!" << endl; break; }

                char randKey[KEY_LENGTH + 1];
                int inserted = 0;
                for (int i = 0; i < n; i++) {
                    generateRandomKey(randKey);
                    if (insertKey(randKey) >= 0)
                        inserted++;
                }
                cout << inserted << " / " << n << " keys inserted." << endl;
                break;
            }

            case 0:
                cout << "Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Enter 0-8." << endl;
        }

    } while (choice != 0);

    return 0;
}
