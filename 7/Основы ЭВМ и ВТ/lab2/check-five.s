@ Lab 2. Signed multiplication by additions and shifts.
@ ARMSim# 1.91, legacy SWI. Five examples from Lab 1.
        .text
        .global _start
_start:
        LDR r8, =inputs
        LDR r9, =results
        MOV r10, #5
next_case:
        LDR r0, [r8], #4
        LDR r1, [r8], #4
        CMP r1, #0
        BGE init
        RSB r0, r0, #0
        RSB r1, r1, #0
init:
        MOV r2, #0
        MOV r3, r0
        MOV r4, r1
        MOV r5, #0
multiply_loop:
        CMP r5, #9
        BGE finish_product
        TST r4, #1
        BEQ shift
        ADD r2, r2, r3
shift:
        MOV r3, r3, LSL #1
        MOV r4, r4, LSR #1
        ADD r5, r5, #1
        B multiply_loop
finish_product:
        MOV r6, r2
        AND r7, r6, #7
        STR r6, [r9], #4
        STR r7, [r9], #4
        LDR r0, =label_p
        SWI 0x02
        MOV r0, #1
        MOV r1, r6
        SWI 0x6b
        LDR r0, =label_r
        SWI 0x02
        MOV r0, #1
        MOV r1, r7
        SWI 0x6b
        MOV r0, #10
        SWI 0x00
        SUBS r10, r10, #1
        BNE next_case
        SWI 0x11
        .data
        .align 2
inputs:
        .word 13, -7, -25, -6, 21, 13, -18, 5, 31, -9
results:
        .skip 40
label_p:
        .asciz "P = "
label_r:
        .asciz "; R = "
        .end
