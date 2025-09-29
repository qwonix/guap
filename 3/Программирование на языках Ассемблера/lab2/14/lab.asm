.386
.model flat, stdcall
.stack 4096

.data
    numbers  WORD 1, 2, 3, 4, 5   ; исходный массив чисел (WORD) - пример входных данных
    nLen     EQU ($ - numbers) / 2  ; количество элементов в массиве
    result   BYTE nLen DUP(0)       ; массив результатов (BYTE) – размер равен числу элементов

.code
main PROC
    mov esi, OFFSET numbers  ; начало исходного массива
    mov edi, OFFSET result   ; начало массива результатов
    mov ecx, nLen            ; количество итераций = число элементов массива

check_loop:
    mov ax, WORD PTR [esi]   ; загружаем число из исходного массива
    test ax, 1               ; проверяем четность, используя операцию test: если младший бит равен 0, число четное
    jnz odd_label            ; если результат не 0 – число нечётное

even_label:
    mov byte ptr [edi], 1    ; четное число – записываем 1 в массив результатов
    jmp next_label           ; переход к следующей итерации

odd_label:
    mov byte ptr [edi], 0    ; нечётное число – записываем 0 в массив результатов

next_label:
    add esi, 2               ; переходим к следующему элементу (WORD = 2 байта)
    add edi, 1               ; следующий элемент результата (BYTE = 1 байт)
    loop check_loop          ; повторяем цикл
main ENDP
end main
