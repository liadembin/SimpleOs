print16:
    pusha
    push si 
    mov si,bx 
; strings will be terminated by 0 byte in memory
print16_loop:
    mov al, [si] ; 'bx' is the base address for the string
    cmp al, 0
    je print16_done

    mov ah, 0x0e ; tty
    mov bl,0
    mov bh,0
    int 0x10 ; 'al' already contains the char

    ; increment pointer and do next loop
    inc si
    jmp print16_loop

print16_done:
    pop si 
    popa
    ret
