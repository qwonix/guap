#!/usr/bin/env kotlin

// determinant.main.kts
// Вычисление определителя матрицы n×m
// Запуск: kotlinc -script determinant.main.kts
//    или: kotlin determinant.main.kts

import kotlin.math.roundToInt

// Глобальные размеры матрицы
var n: Int = 0
var m: Int = 0

// ─────────────────────────────────────────────
// Функция: запрос размеров матрицы у пользователя
// ─────────────────────────────────────────────
fun inputSize() {
    print("Введите количество строк n: ")
    n = readln().trim().toIntOrNull() ?: 0
    while (n <= 0) {
        print("n должно быть больше 0. Введите снова: ")
        n = readln().trim().toIntOrNull() ?: 0
    }

    print("Введите количество столбцов m: ")
    m = readln().trim().toIntOrNull() ?: 0
    while (m <= 0) {
        print("m должно быть больше 0. Введите снова: ")
        m = readln().trim().toIntOrNull() ?: 0
    }
}

// ─────────────────────────────────────────────
// Функция: заполнение матрицы вручную
// ─────────────────────────────────────────────
fun fillMatrix(a: Array<IntArray>) {
    println("Введите элементы матрицы ${n}x${m}:")
    for (i in 0 until n) {
        for (j in 0 until m) {
            print("A[${i + 1}][${j + 1}] = ")
            a[i][j] = readln().trim().toIntOrNull() ?: 0
        }
    }
}

// ─────────────────────────────────────────────
// Функция: заполнение матрицы случайными числами (от -9 до 9)
// ─────────────────────────────────────────────
fun fillMatrixRandom(a: Array<IntArray>) {
    val rng = java.util.Random()
    for (i in 0 until n) {
        for (j in 0 until m) {
            a[i][j] = rng.nextInt(19) - 9   // числа от -9 до 9
        }
    }
    println("Матрица заполнена случайными числами.")
}

// ─────────────────────────────────────────────
// Функция: вывод матрицы на экран
// ─────────────────────────────────────────────
fun printMatrix(a: Array<IntArray>) {
    println("\nМатрица ${n}x${m}:")
    for (i in 0 until n) {
        println(a[i].joinToString("\t"))
    }
}

// ─────────────────────────────────────────────
// Функция: вычисление определителя методом Гаусса
// Принимает матрицу Int, внутри работает с Double для точности
// Возвращает Int (определитель целочисленной матрицы — целое число)
// ─────────────────────────────────────────────
fun calcDeterminant(a: Array<IntArray>): Int {
    // Копируем в Double чтобы деление не теряло точность
    val b = Array(n) { i -> DoubleArray(n) { j -> a[i][j].toDouble() } }

    var det = 1.0

    for (col in 0 until n) {
        // Ищем ненулевой опорный элемент в столбце
        val pivotRow = (col until n).firstOrNull { b[it][col] != 0.0 }

        // Весь столбец нулевой — определитель равен 0
        if (pivotRow == null) return 0

        // Меняем строки местами, если нужно
        if (pivotRow != col) {
            val tmp = b[col]; b[col] = b[pivotRow]; b[pivotRow] = tmp
            det *= -1.0
        }

        det *= b[col][col]

        // Обнуляем элементы ниже опорного
        for (row in col + 1 until n) {
            val factor = b[row][col] / b[col][col]
            for (k in col until n) {
                b[row][k] -= factor * b[col][k]
            }
        }
    }

    return det.roundToInt()
}

// ─────────────────────────────────────────────
// Функция: вывод результата
// ─────────────────────────────────────────────
fun printResult(det: Int) {
    println("\nОпределитель матрицы = $det")
}

// ─────────────────────────────────────────────
// Функция: меню программы
// ─────────────────────────────────────────────
fun showMenu() {
    println("""
=== МЕНЮ ===
1. Ввести матрицу вручную
2. Заполнить матрицу случайными числами
3. Вывести матрицу
4. Вычислить определитель
0. Выход
""".trimIndent())
    print("Выберите действие: ")
}

// ─────────────────────────────────────────────
// Точка входа
// ─────────────────────────────────────────────
inputSize()
val matrix = Array(n) { IntArray(m) }
var filled = false

do {
    showMenu()
    val choice = readln().trim().toIntOrNull() ?: -1

    when (choice) {
        1 -> {
            fillMatrix(matrix)
            filled = true
            println("Матрица введена.")
        }
        2 -> {
            fillMatrixRandom(matrix)
            filled = true
        }
        3 -> {
            if (!filled) println("Сначала заполните матрицу (пункт 1 или 2).")
            else printMatrix(matrix)
        }
        4 -> {
            when {
                !filled -> println("Сначала заполните матрицу (пункт 1 или 2).")
                n != m  -> println("Определитель существует только для квадратных матриц (n == m).\nТекущий размер: ${n}x${m} — не квадратная.")
                else    -> printResult(calcDeterminant(matrix))
            }
        }
        0 -> println("Выход из программы.")
        else -> println("Неверный выбор. Попробуйте снова.")
    }
} while (choice != 0)