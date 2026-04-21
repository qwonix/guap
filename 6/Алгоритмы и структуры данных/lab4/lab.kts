import java.io.File
import java.util.*
import kotlin.system.exitProcess

// ============================================================
// Константы
// ============================================================
const val NUM_SEGMENTS = 3000
const val KEY_LENGTH = 6

// ============================================================
// Состояние ячейки
// ============================================================
enum class CellState {
    EMPTY,
    OCCUPIED,
    DELETED
}

// ============================================================
// Ячейка таблицы
// ============================================================
data class HashCell(
    var key: String = "",
    var state: CellState = CellState.EMPTY
)

// ============================================================
// Глобальные структуры
// ============================================================
val hashTable = Array(NUM_SEGMENTS) { HashCell() }
val hitCount = IntArray(NUM_SEGMENTS)

// ============================================================
// Хеш-функция (полиномиальная)
// ============================================================
fun hashFunction(key: String): Int {
    val p = 31L
    var hash = 0L
    var power = 1L

    for (i in KEY_LENGTH - 1 downTo 0) {
        hash = (hash + key[i].code * power % NUM_SEGMENTS) % NUM_SEGMENTS
        power = (power * p) % NUM_SEGMENTS
    }

    return hash.toInt()
}

// ============================================================
// Проверка формата ключа
// ============================================================
fun isValidKey(key: String): Boolean {
    if (key.length != KEY_LENGTH) return false
    if (!key[0].isUpperCase()) return false
    if (!key[5].isUpperCase()) return false

    for (i in 1..4) {
        if (!key[i].isDigit()) return false
    }

    return true
}

// ============================================================
// Квадратичное опробование
// ============================================================
fun quadraticProbe(base: Int, step: Int): Int {
    return ((base + step.toLong() * step) % NUM_SEGMENTS).toInt()
}

// ============================================================
// Вставка
// ============================================================
fun insertKey(key: String): Int {
    if (!isValidKey(key)) {
        println("Invalid key format!")
        return -1
    }

    val base = hashFunction(key)
    var insertAt = -1
    var firstDeleted = -1

    for (step in 0 until NUM_SEGMENTS) {
        val probe = quadraticProbe(base, step)
        hitCount[probe]++

        when (hashTable[probe].state) {
            CellState.EMPTY -> {
                insertAt = if (firstDeleted != -1) firstDeleted else probe
                break
            }

            CellState.DELETED -> {
                if (firstDeleted == -1) firstDeleted = probe
            }

            CellState.OCCUPIED -> {
                if (hashTable[probe].key == key) {
                    println("Key already exists!")
                    return -1
                }
            }
        }
    }

    if (insertAt == -1 && firstDeleted != -1)
        insertAt = firstDeleted

    if (insertAt == -1) {
        println("Table is FULL!")
        return -1
    }

    hashTable[insertAt] = HashCell(key, CellState.OCCUPIED)
    return insertAt
}

// ============================================================
// Поиск по ключу
// ============================================================
fun searchByKey(key: String): Int {
    if (!isValidKey(key)) return -1

    val base = hashFunction(key)

    for (step in 0 until NUM_SEGMENTS) {
        val probe = quadraticProbe(base, step)

        when (hashTable[probe].state) {
            CellState.EMPTY -> return -1
            CellState.OCCUPIED -> {
                if (hashTable[probe].key == key) return probe
            }
            else -> {}
        }
    }

    return -1
}

// ============================================================
// Поиск по сегменту
// ============================================================
fun searchBySegment(seg: Int) {
    if (seg !in 0 until NUM_SEGMENTS) {
        println("Invalid segment!")
        return
    }

    val cell = hashTable[seg]
    when (cell.state) {
        CellState.EMPTY -> println("Empty")
        CellState.DELETED -> println("Deleted")
        CellState.OCCUPIED -> println("Key: ${cell.key}")
    }
}

// ============================================================
// Удаление
// ============================================================
fun deleteKey(key: String) {
    if (!isValidKey(key)) return

    val base = hashFunction(key)
    var foundIndex = -1

    for (step in 0 until NUM_SEGMENTS) {
        val probe = quadraticProbe(base, step)

        if (hashTable[probe].state == CellState.EMPTY) break

        if (hashTable[probe].state == CellState.OCCUPIED &&
            hashTable[probe].key == key
        ) {
            foundIndex = probe
            break
        }
    }

    if (foundIndex == -1) {
        println("Key not found")
        return
    }

    hashTable[foundIndex] = HashCell("", CellState.DELETED)
    println("Deleted at $foundIndex")

    // поиск коллизий
    println("Collisions:")
    for (i in 0 until NUM_SEGMENTS) {
        val cell = hashTable[i]
        if (cell.state == CellState.OCCUPIED) {
            if (hashFunction(cell.key) == base) {
                println("  ${cell.key} at $i")
            }
        }
    }
}

// ============================================================
// Просмотр таблицы
// ============================================================
fun printTable() {
    var count = 0
    for (i in 0 until NUM_SEGMENTS) {
        val cell = hashTable[i]
        if (cell.state == CellState.OCCUPIED) {
            println("[$i] ${cell.key}")
            count++
        }
    }
    println("Total: $count")
}

// ============================================================
// Экспорт
// ============================================================
fun exportCSV(filename: String) {
    val file = File(filename)
    file.printWriter().use { out ->
        out.println("Segment;Hits;Key")
        for (i in 0 until NUM_SEGMENTS) {
            val key = if (hashTable[i].state == CellState.OCCUPIED)
                hashTable[i].key else ""
            out.println("$i;${hitCount[i]};$key")
        }
    }
    println("Exported to $filename")
}

// ============================================================
// Загрузка
// ============================================================
fun loadFromFile(filename: String) {
    val file = File(filename)
    if (!file.exists()) {
        println("File not found")
        return
    }

    var count = 0

    file.readLines().drop(1).forEach {
        val parts = it.split(";")
        if (parts.size >= 3) {
            val key = parts[2].trim()
            if (isValidKey(key)) {
                if (insertKey(key) >= 0) count++
            }
        }
    }

    println("Loaded $count keys")
}

// ============================================================
// Генерация ключа
// ============================================================
fun generateRandomKey(): String {
    val rand = Random()
    return buildString {
        append(('A' + rand.nextInt(26)))
        repeat(4) { append(rand.nextInt(10)) }
        append(('A' + rand.nextInt(26)))
    }
}

// ============================================================
// Меню
// ============================================================
fun main() {
    val scanner = Scanner(System.`in`)

    while (true) {
        println("\n1.Insert 2.Search 3.Segment 4.Delete 5.View 6.Export 7.Load 8.Random 0.Exit")

        when (scanner.nextInt()) {
            1 -> {
                print("Key: ")
                val key = scanner.next()
                println("Inserted at ${insertKey(key)}")
            }

            2 -> {
                print("Key: ")
                val key = scanner.next()
                println("Found at ${searchByKey(key)}")
            }

            3 -> {
                print("Segment: ")
                searchBySegment(scanner.nextInt())
            }

            4 -> {
                print("Key: ")
                deleteKey(scanner.next())
            }

            5 -> printTable()

            6 -> {
                print("File: ")
                exportCSV(scanner.next())
            }

            7 -> {
                print("File: ")
                loadFromFile(scanner.next())
            }

            8 -> {
                print("Count: ")
                val n = scanner.nextInt()
                repeat(n) { insertKey(generateRandomKey()) }
            }

            0 -> exitProcess(0)
        }
    }
}