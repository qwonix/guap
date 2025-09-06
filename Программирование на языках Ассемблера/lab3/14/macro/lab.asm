.386
.model flat, stdcall
.stack 4096

.data
    numbers  WORD 2, 2, 1, 4, 10    ; исходный массив чисел (WORD) - пример входных данных
    nLen     EQU ($ - numbers) / 2  ; количество элементов в массиве
    result   BYTE nLen DUP(0)       ; массив результатов (BYTE) – размер равен числу элементов

.code

check_even MACRO n1     ; объявление макроса
    test n1, 1          ; проверяем четность, используя операцию test: если младший бит равен 0, число четное
    sete al             ; устанавливаем результат в al
ENDM

main PROC
    mov esi, OFFSET numbers  ; начало исходного массива
    mov edi, OFFSET result   ; начало массива результатов
    mov ecx, nLen            ; количество итераций = число элементов массива

check_loop:
    mov ax, WORD PTR [esi] ; загружаем число из исходного массива 
    check_even ax          ; вызов макроса
    mov [edi], al          ; записываем результат в массив результатов

    add esi, 2             ; переходим к следующему элементу (WORD = 2 байта)
    add edi, 1             ; следующий элемент результата (BYTE = 1 байт)
    loop check_loop        ; повторяем цикл

main ENDP
end main