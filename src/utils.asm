; utils.asm
extern printf

section .data
    format_registers db "rbx:0x%lx r12:0x%lx r13:%ld r14:%ld", 10, 0
    format_stack db "sommet (rsp): 0x%lx, base du bloc (rbp): 0x%lx", 10, 0

section .text

global print_stack
global print_registers

print_registers:
    push rbp
    mov rbp, rsp
    
    mov r8,  r14
    mov rcx, r13
    mov rdx, r12
    mov rsi, rbx
    mov rdi, format_registers
    mov rax, 0
    call printf 
        
    pop rbp
    ret

print_stack:
    push rbp
    mov rbp, rsp

    mov rdx, [rsp]
    mov rax, rsp
    add rax, 16
    mov rsi, rax
    mov rdi, format_stack
    mov rax, 0
    call printf WRT ..plt
    
    pop rbp
    ret
