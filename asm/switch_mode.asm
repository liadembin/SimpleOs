[bits 16]
switch_mode:
    cli;disable 16bit interupts
    lgdt [gdt_descriptor]
    mov eax,cr0
    or eax,0x1;allow protected mode 
    mov cr0,eax
    jmp CODE_SEG:init_32bit
    ret
%include "print_32bit.asm"
[bits 32]
init_32bit:
    mov ax, DATA_SEG        ; 5. update segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; 6. setup stack
    mov esp, ebp
    
    mov  ebx,switched
    call print32
    call BEGIN_32BIT        ; 7. move back to mbr.asm
switched db 10,13,"I Switched fully...",0
