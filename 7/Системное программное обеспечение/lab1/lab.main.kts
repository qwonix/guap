#!/usr/bin/env kotlin

import java.io.File

// Элемент основной таблицы идентификаторов.
// next — индекс следующего элемента в цепочке.
// Значение -1 означает конец цепочки.
data class Entry(
    val name: String,
    var next: Int = -1
)

// Результат поиска.
data class SearchResult(
    val found: Boolean,
    val comparisons: Int
)

class HashTable(
    private val size: Int
) {

    // Хеш-таблица.
    // В каждой ячейке хранится индекс первого элемента цепочки.
    // -1 означает, что цепочка пуста.
    private val buckets = IntArray(size) { -1 }

    // Основная таблица идентификаторов.
    // Новые элементы добавляются в конец.
    private val entries = mutableListOf<Entry>()

    // Общее количество коллизий при вставке.
    private var totalCollisions = 0

    // Количество успешно добавленных идентификаторов.
    private var insertedCount = 0

    // Хеш-функция:
    // код первой буквы + код последней буквы.
    // Остаток от деления используется для получения
    // корректного индекса в таблице.
    private fun hash(name: String): Int {
        val firstCode = name.first().code
        val lastCode = name.last().code

        return (firstCode + lastCode) % size
    }

    // Вставка нового идентификатора.
    fun insert(name: String): Boolean {

        if (name.isEmpty()) {
            return false
        }

        // Проверяем, нет ли уже такого идентификатора.
        val existing = search(name)

        if (existing.found) {
            return false
        }

        val hashIndex = hash(name)

        // Если в ячейке уже есть элемент,
        // происходит коллизия.
        if (buckets[hashIndex] != -1) {
            totalCollisions++
        }

        // Индекс нового элемента в основной таблице.
        val newIndex = entries.size

        // Добавляем идентификатор.
        entries.add(Entry(name))

        // Если цепочка пустая,
        // новый элемент становится первым.
        if (buckets[hashIndex] == -1) {
            buckets[hashIndex] = newIndex
        } else {

            // Иначе идём по цепочке до последнего элемента.
            var currentIndex = buckets[hashIndex]

            while (entries[currentIndex].next != -1) {
                currentIndex = entries[currentIndex].next
            }

            // Добавляем новый элемент в конец цепочки.
            entries[currentIndex].next = newIndex
        }

        insertedCount++

        return true
    }

    // Поиск идентификатора.
    fun search(name: String): SearchResult {

        if (name.isEmpty()) {
            return SearchResult(false, 0)
        }

        val hashIndex = hash(name)

        // Получаем первый элемент цепочки.
        var currentIndex = buckets[hashIndex]

        var comparisons = 0

        // Просматриваем цепочку.
        while (currentIndex != -1) {

            comparisons++

            if (entries[currentIndex].name == name) {
                return SearchResult(true, comparisons)
            }

            currentIndex = entries[currentIndex].next
        }

        return SearchResult(false, comparisons)
    }

    // Вывод содержимого таблицы.
    fun printTable() {

        println()
        println("HASH TABLE")
        println("=".repeat(60))

        println("Hash index | Chain")
        println("-".repeat(60))

        for (i in buckets.indices) {

            print("%10d | ".format(i))

            var currentIndex = buckets[i]

            if (currentIndex == -1) {
                println("empty")
                continue
            }

            while (currentIndex != -1) {

                val entry = entries[currentIndex]

                print("[${currentIndex}: ${entry.name}]")

                if (entry.next != -1) {
                    print(" -> ")
                }

                currentIndex = entry.next
            }

            println()
        }

        println("-".repeat(60))

        println()
        println("MAIN IDENTIFIER TABLE")
        println("=".repeat(60))

        println("Index | Identifier | Next")
        println("-".repeat(60))

        for (i in entries.indices) {
            println(
                "%5d | %-10s | %4d".format(
                    i,
                    entries[i].name,
                    entries[i].next
                )
            )
        }

        println("-".repeat(60))
    }

    // Среднее количество коллизий.
    fun getAverageCollisions(): Double {
        if (insertedCount == 0) {
            return 0.0
        }

        return totalCollisions.toDouble() / insertedCount
    }

    // Получение количества коллизий.
    fun getTotalCollisions(): Int {
        return totalCollisions
    }

    // Получение количества элементов.
    fun getSize(): Int {
        return insertedCount
    }
}


// Размер хеш-таблицы.
val tableSize = 13

val hashTable = HashTable(tableSize)

val fileName = "identifiers.txt"

println("HASH TABLE LABORATORY WORK")
println("Variant: 12")
println("Hash function: first character code + last character code")
println("Collision resolution: chaining")
println()

// Читаем идентификаторы из файла.
val identifiers = try {
    File(fileName)
        .readLines()
        .map { it.trim() }
        .filter { it.isNotEmpty() }
} catch (e: Exception) {
    println("Error: file '$fileName' was not found.")
    throw e
}

// Добавляем идентификаторы в таблицу.
for (identifier in identifiers) {

    if (identifier.length > 32) {
        println(
            "Identifier '$identifier' was skipped: length is greater than 32."
        )
        continue
    }

    val inserted = hashTable.insert(identifier)

    if (!inserted) {
        println("Identifier '$identifier' was not added.")
    }
}

// Показываем таблицу.
hashTable.printTable()

// Статистика коллизий.
println()
println("STATISTICS")
println("=".repeat(60))

println("Number of identifiers: ${hashTable.getSize()}")
println("Total collisions: ${hashTable.getTotalCollisions()}")
println(
    "Average collisions: %.2f".format(
        hashTable.getAverageCollisions()
    )
)

println("=".repeat(60))

// Поиск идентификаторов.
println()
println("SEARCH")
println("Enter identifier to search.")
println("Enter 'exit' to finish.")

// Статистика сравнений только для запросов интерактивного поиска.
var searchCount = 0
var totalSearchComparisons = 0

while (true) {

    print("> ")

    val input = readlnOrNull()?.trim() ?: break

    if (input.equals("exit", ignoreCase = true)) {
        break
    }

    if (input.isEmpty()) {
        continue
    }

    val result = hashTable.search(input)
    searchCount++
    totalSearchComparisons += result.comparisons

    if (result.found) {
        println(
            "Identifier '$input' was found. " +
                    "Comparisons: ${result.comparisons}"
        )
    } else {
        println(
            "Identifier '$input' was not found. " +
                    "Comparisons: ${result.comparisons}"
        )
    }
}

// Среднее количество сравнений за один выполненный поиск.
val averageSearchComparisons = if (searchCount == 0) {
    0.0
} else {
    totalSearchComparisons.toDouble() / searchCount
}

println()
println("SEARCH STATISTICS")
println("=".repeat(60))
println("Number of searches: $searchCount")
println("Total comparisons: $totalSearchComparisons")
println("Average comparisons per search: %.2f".format(averageSearchComparisons))
println("=".repeat(60))

println()
println("Program finished.")
