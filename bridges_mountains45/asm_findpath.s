.syntax unified
.arm
.text
.global asm_findpath
.type asm_findpath, %function
@ void asm_findpath(int* grid, int width, int height, int start_row, int max_value)
@ Mark a greedy west-to-east path with -1. Legacy max_value is unused.
@ Elevations must be nonnegative int32 values. Ties: east, northeast, southeast.
asm_findpath:
    push {r4-r11, lr}
    cmp r0, #0
    beq .Lreturn
    cmp r1, #0
    ble .Lreturn
    cmp r2, #0
    ble .Lreturn
    cmp r3, #0
    blt .Lreturn
    cmp r3, r2
    bge .Lreturn
    mov r4, r0
    mov r5, r1                  @ Width.
    mov r6, r2                  @ Height.
    mov r7, r3                  @ Current row.
    mov r8, #0                  @ Current column.
.Lcolumn:
    mla r9, r7, r5, r8          @ Element index, not byte offset.
    ldr r10, [r4, r9, lsl #2]   @ Read elevation before overwriting it.
    mvn r0, #0
    str r0, [r4, r9, lsl #2]
    add r8, r8, #1
    cmp r8, r5
    bge .Lreturn               @ Mark the final column too.
    add r9, r9, #1             @ Candidate east.
    ldr r0, [r4, r9, lsl #2]
    subs r11, r0, r10
    rsbmi r11, r11, #0         @ Best absolute elevation change.
    mov r12, r7
    cmp r7, #0
    beq .Lsoutheast
    sub r1, r9, r5
    ldr r0, [r4, r1, lsl #2]
    subs r2, r0, r10
    rsbmi r2, r2, #0
    cmp r2, r11
    movlt r11, r2
    sublt r12, r7, #1
.Lsoutheast:
    add r1, r7, #1
    cmp r1, r6
    bge .Lchosen
    add r2, r9, r5
    ldr r0, [r4, r2, lsl #2]
    subs r2, r0, r10
    rsbmi r2, r2, #0
    cmp r2, r11
    movlt r12, r1
.Lchosen:
    mov r7, r12
    b .Lcolumn
.Lreturn:
    pop {r4-r11, pc}
.size asm_findpath, .-asm_findpath
.section .note.GNU-stack,"",%progbits
