.syntax unified
.arm
.text
.global fizzbuzz
.type fizzbuzz, %function
fizzbuzz:
    push {r4-r8, lr}             @ 24 bytes: public stack remains aligned.
    mov r4, #1
.Lnumber:
    cmp r4, #100
    bgt .Ldone
    mov r5, r4
.Lmod15:
    cmp r5, #15
    subge r5, r5, #15
    bge .Lmod15
    cmp r5, #0
    beq .Lfizzbuzz
    mov r5, r4
.Lmod3:
    cmp r5, #3
    subge r5, r5, #3
    bge .Lmod3
    cmp r5, #0
    beq .Lfizz
    mov r5, r4
.Lmod5:
    cmp r5, #5
    subge r5, r5, #5
    bge .Lmod5
    cmp r5, #0
    beq .Lbuzz
    mov r0, r4
    bl print_number
    b .Lnext
.Lfizzbuzz:
    bl print_fizzbuzz
    b .Lnext                    @ Calls may overwrite condition flags.
.Lfizz:
    bl print_fizz
    b .Lnext
.Lbuzz:
    bl print_buzz
.Lnext:
    add r4, r4, #1
    b .Lnumber
.Ldone:
    pop {r4-r8, pc}
.size fizzbuzz, .-fizzbuzz
.section .note.GNU-stack,"",%progbits
