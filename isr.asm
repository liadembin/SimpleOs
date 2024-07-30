[bits 32]
[extern isr_handler]
extern isr_handler


global isr0
isr0:
    cli
    push dword 0
    push dword 0
    mov edx,0xFF
    jmp isr_common_stub

global isr1
isr1:
    cli
    push byte 0
    push byte 1
    jmp isr_common_stub

global isr2
isr2:
    cli
    push byte 0
    push byte 2
    jmp isr_common_stub

global isr3
isr3:
    cli
    push byte 0
    push byte 3
    jmp isr_common_stub

global isr4
isr4:
    cli
    push byte 0
    push byte 4
    jmp isr_common_stub

global isr5
isr5:
    cli
    push byte 0
    push byte 5
    jmp isr_common_stub

global isr6
isr6:
    cli
    push byte 0
    push byte 6
    jmp isr_common_stub

global isr7
isr7:
    cli
    push byte 0
    push byte 7
    jmp isr_common_stub

global isr8
isr8:
    cli
    push byte 0
    push byte 8
    jmp isr_common_stub

global isr9
isr9:
    cli
    push byte 0
    push byte 9
    jmp isr_common_stub

global isr10
isr10:
    cli
    push byte 0
    push byte 10
    jmp isr_common_stub

global isr11
isr11:
    cli
    push byte 0
    push byte 11
    jmp isr_common_stub

global isr12
isr12:
    cli
    push byte 0
    push byte 12
    jmp isr_common_stub

global isr13
isr13:
    cli
    push byte 0
    push byte 13
    jmp isr_common_stub

global isr14
isr14:
    cli
    push byte 0
    push byte 14
    jmp isr_common_stub

global isr15
isr15:
    cli
    push byte 0
    push byte 15
    jmp isr_common_stub

global isr16
isr16:
    cli
    push byte 0
    push byte 16
    jmp isr_common_stub

global isr17
isr17:
    cli
    push byte 0
    push byte 17
    jmp isr_common_stub

global isr18
isr18:
    cli
    push byte 0
    push byte 18
    jmp isr_common_stub

global isr19
isr19:
    cli
    push byte 0
    push byte 19
    jmp isr_common_stub

global isr20
isr20:
    cli
    push byte 0
    push byte 20
    jmp isr_common_stub

global isr21
isr21:
    cli
    push byte 0
    push byte 21
    jmp isr_common_stub

global isr22
isr22:
    cli
    push byte 0
    push byte 22
    jmp isr_common_stub

global isr23
isr23:
    cli
    push byte 0
    push byte 23
    jmp isr_common_stub

global isr24
isr24:
    cli
    push byte 0
    push byte 24
    jmp isr_common_stub

global isr25
isr25:
    cli
    push byte 0
    push byte 25
    jmp isr_common_stub

global isr26
isr26:
    cli
    push byte 0
    push byte 26
    jmp isr_common_stub

global isr27
isr27:
    cli
    push byte 0
    push byte 27
    jmp isr_common_stub

global isr28
isr28:
    cli
    push byte 0
    push byte 28
    jmp isr_common_stub

global isr29
isr29:
    cli
    push byte 0
    push byte 29
    jmp isr_common_stub

global isr30
isr30:
    cli
    push byte 0
    push byte 30
    jmp isr_common_stub

global isr31
isr31:
    cli
    push byte 0
    push byte 31
    jmp isr_common_stub

global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15
irq0:
cli
push byte 0
push byte 33
jmp isr_common_stub
irq1:
cli
push byte 1
push byte 34
jmp isr_common_stub
irq2:
cli
push byte 2
push byte 35
jmp isr_common_stub
irq3:
cli
push byte 3
push byte 36
jmp isr_common_stub
irq4:
cli
push byte 4
push byte 37
jmp isr_common_stub
irq5:
cli
push byte 5
push byte 38
jmp isr_common_stub
irq6:
cli
push byte 6
push byte 39
jmp isr_common_stub
irq7:
cli
push byte 7
push byte 40
jmp isr_common_stub
irq8:
cli
push byte 8
push byte 41
jmp isr_common_stub
irq9:
cli
push byte 9
push byte 42
jmp isr_common_stub
irq10:
cli
push byte 10
push byte 43
jmp isr_common_stub
irq11:
cli
push byte 11
push byte 44
jmp isr_common_stub
irq12:
cli
push byte 12
push byte 45
jmp isr_common_stub
irq13:
cli
push byte 13
push byte 46
jmp isr_common_stub
irq14:
cli
push byte 14
push byte 47
jmp isr_common_stub
irq15:
cli
push byte 15
push byte 48
jmp isr_common_stub
isr_common_stub:
    ;outside push is interupt_number
    ;then err_code 
    
    pushad
    
    mov ax, ds
    push eax
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    push esp
    call isr_handler
    pop eax 
    pop eax 

    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popad
    add esp, 8
    sti
    iret

