.data
    text byte "Hello, world!", 10, 0    ; определяем выводимые данные

.code
externdef origfunc:word
externdef hookfunc:proc
externdef shadow: proc

public MultiplyBy10
MultiplyBy10  PROC
    pop r11
    push rcx
    push rdx
    push r8
    push r9
    sub rsp, 40    ; резервируем в стеке 40 байт
    mov rcx, r11
    call hookfunc    ; вызываем функцию hookfunc
    mov r11, rax
    add rsp, 40    ; восстанавливаем значение в стеке
    pop r9
    pop r8
    pop rdx
    pop rcx
    
    mov rax,0
   
    jmp r11

    ;push rax
    ;push rdx
    ;sub rsp, 40
    ;mov rcx, 0
    ;call perehook
    ;add rsp, 40
    ;pop rdx
    ;pop rax
    
    ;jmp 
 
    ret
 
MultiplyBy10  ENDP


public Shadowfile
Shadowfile  PROC
    pop r11
    push rcx
    push rdx
    push r8
    push r9
    sub rsp, 40    ; резервируем в стеке 40 байт
    mov rcx, r11
    call hookfunc    ; вызываем функцию hookfunc
    mov r11, rax
    add rsp, 40    ; восстанавливаем значение в стеке
    pop r9
    pop r8
    pop rdx
    pop rcx

    push r11
    push rcx
    push rdx
    push r8
    push r9
    sub rsp, 40 
    call shadow
    add rsp, 40    ; восстанавливаем значение в стеке
    pop r9
    pop r8
    pop rdx
    pop rcx
    pop r11

    test rax, rax
    jne yes
    ret
   
yes:
    jmp r11
    ret

    ;mov rax,0
   
    ;jmp r11

    ;push rax
    ;push rdx
    ;sub rsp, 40
    ;mov rcx, 0
    ;call perehook
    ;add rsp, 40
    ;pop rdx
    ;pop rax
    
    ;jmp 
 
    ;ret
 
Shadowfile  ENDP
 
END