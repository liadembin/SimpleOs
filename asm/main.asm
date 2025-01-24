[bits 16]
[org 0x7c00]
KERNEL_OFFSET equ 0x1000
; BIOS sets boot drive in 'dl'; store for later use
mov [BOOT_DRIVE], dl

mov bp, 0xFF00
mov sp, bp
mov bx,starting
call set_mode 

call print16
call load_kernel
;mov bx,switch_mode
; call print16
jmp $
%include "./asm/gdt.asm"
%include "./asm/print_16bit.asm"
%include "./asm/switch_mode.asm"

[bits 16]
set_mode:
    pusha 
    mov ah,0 
    mov al,03 
    int 10h

    popa 
    ret
load_kernel:
    mov bx,KERNEL_OFFSET 
    mov al,54;SECTOR_COUNT_CONST;3 sectors 
    mov [SECTOR_COUNT],al
    mov dl,[BOOT_DRIVE] 
    call load_disk
    mov bx,switching
    call print16
    call switch_mode
    ret
load_disk:
    pusha
    ;input is: 
    ; bx offset
    ; al number of sectors 
    ; dl is the drive
    ;push ax 
    ;for the int 
    ;ah = 2 
    ; al = number of sectors 
    ; ch = track number(0)
    ; cl = sector number(2)
    ; dh = head number(0)
    ; dl = drive 
    ; es:bx where to read 
    ;returns: CF is set if error,   ah status, al amount sectors read.
    mov ah,02
    mov ch,0
    mov cl,2
    mov dh,0
    ;push ax 
        ; push ax 
        ;     mov ax,0
        ;     mov es,ax
        ; pop ax 
        ;
        mov bx,KERNEL_OFFSET
        int 0x13
        jc disk_error
    ;pop bx 
    cmp [SECTOR_COUNT],al 
    jne not_all_read
    ;
    popa
    ret
disk_error:
    mov bx,disk_error_msg
    call print16
    jmp $; print a error msg later...
not_all_read:
    
    mov bx,not_all_read_msg
    add ah,'0'
    mov [code],ah
    add al,'0'
    mov [Number],al
    call print16

    popa
    ret
    jmp $; print a error msg later 

[bits 32]
; %include "print_32bit.asm"
BEGIN_32BIT:
    mov ebx,starting_msg
    call print32
    call KERNEL_OFFSET ; give control to the kernel

    jmp $ ; loop in case kernel returns


.end:
    starting_msg db "Starting Fully now",0
    starting db "Begining to read disk",0
    switching db "All read succsessfully. Begining to Switch",0
        not_all_read_msg db "Not all sectors were read  "
    Error db "Error: "
    code db "Z  Number read: "
    Number db "0",0
    disk_error_msg db 10,13,"Disk Error!!!!!",0
    switching_msg db "Switching mode",0
    BOOT_DRIVE db 0
    SECTOR_COUNT db 0
    times 510 - ($ -$$) db 0 ;bassicly 100 dup(0)
    dw 0xaa55
