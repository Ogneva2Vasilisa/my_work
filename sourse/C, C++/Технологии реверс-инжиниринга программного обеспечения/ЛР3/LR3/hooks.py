from angrutils import *
import lief

length = []
buf_addr = []
hook_addr = []
sink_addr = list()

filename = input("Введите название файла:\n")

def hook_gets(state: angr.SimState):
    global length
    global buf_addr
    size = state.regs.rdx
    buffer = state.regs.rcx
    if claripy.backends.concrete.convert(buffer).value not in buf_addr:
        length.append(claripy.backends.concrete.convert(size).value)
        buf_addr.append(claripy.backends.concrete.convert(buffer).value)
    #print(hex(buf_addr), hex(state.addr))

def readfile_hook(state: angr.SimState):
    global length
    global buf_addr
    buffer = state.regs.rdx
    size = state.regs.r8d
    if claripy.backends.concrete.convert(buffer).value not in buf_addr:
        length.append(claripy.backends.concrete.convert(size).value)
        buf_addr.append(claripy.backends.concrete.convert(buffer).value)

def readfile_hook_86(state: angr.SimState):
    global length
    global buf_addr
    buffer = state.regs.edx
    size = state.regs.r8d
    if claripy.backends.concrete.convert(buffer).value not in buf_addr:
        length.append(claripy.backends.concrete.convert(size).value)
        buf_addr.append(claripy.backends.concrete.convert(buffer).value)

def hook_reg_open_key_exw(state : angr.SimState):
    hKey = state.solver.eval(state.regs.rcx)  # Первый аргумент (HKEY)
    lpSubKey_ptr = state.solver.eval(state.regs.rdx)  # Второй аргумент (LPCWSTR)
    ulOptions = state.solver.eval(state.regs.r8)  # Третий аргумент (DWORD)
    samDesired = state.solver.eval(state.regs.r9)  # Четвертый аргумент (REGSAM)

    # Фиктивный дескриптор ключа (возвращаемое значение в phkResult)
    phkResult_ptr = state.mem[state.regs.rbp + 0x10].int.resolved

    # Логирование

    global filename
    namekey = ''
    binary = lief.parse(filename)
    flag = 0
    for i in binary.get_content_from_virtual_address(lpSubKey_ptr, 100):
        if i == 0:
            if flag == 0:
                flag = 1
            else:
                break
        else:
            flag = 0
            namekey += chr(i)
            #print(chr(i), end='')
    #print()

    print(f"Hooked RegOpenKeyExW: "
          f"hKey={hKey}, lpSubKey_ptr={namekey}, ulOptions={ulOptions}, samDesired={samDesired}")

    # Установка фиктивного значения для phkResult (например, дескриптор 0x1234)
    state.memory.store(phkResult_ptr, claripy.BVV(0x1234, 64))  # 64-битный дескриптор

    # Возвращаемый результат (ERROR_SUCCESS = 0)
    state.regs.rax = claripy.BVV(0, 64)  # Возвращает 0 (успех)

def hook_reg_query_value_exw(state):
    hKey = state.solver.eval(state.regs.rcx)  # Первый аргумент (HKEY)
    lpValueName_ptr = state.solver.eval(state.regs.rdx)  # Второй аргумент (LPCWSTR)
    lpReserved = state.solver.eval(state.regs.r8)  # Третий аргумент
    lpType_ptr = state.solver.eval(state.regs.r9)  # Четвертый аргумент

    # Имитация данных о результатах запроса реестра
    lpData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x28, 8))
    pcbData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x30, 8))

    global filename
    namekey = ''
    binary = lief.parse(filename)
    flag = 0
    for i in binary.get_content_from_virtual_address(lpValueName_ptr, 100):
        if i == 0:
            if flag == 0:
                flag = 1
            else:
                break
        else:
            flag = 0
            namekey += chr(i)

    # Логирование
    print(f"Hooked RegQueryValueExW: "
          f"hKey={hex(hKey)}, lpValueName_ptr={namekey}")

    state.regs.rax = claripy.BVV(0, 64)  # Возвращает 0 (успех)

def hook_reg_get_value_w(state):
    hKey = state.solver.eval(state.regs.rcx)  # Первый аргумент (HKEY)
    lpSubKey_ptr = state.solver.eval(state.regs.rdx)  # Второй аргумент (LPCWSTR)
    lpValue_ptr = state.solver.eval(state.regs.r8)  # Третий аргумент
    dwFlags = state.solver.eval(state.regs.r9)  # Четвертый аргумент (флаги)

    lpData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x28, 8))
    pcbData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x40, 8))
    # print(hex(lpData_ptr), hex(pcbData_ptr))

    global filename
    namekey = ''
    binary = lief.parse(filename)
    flag = 0
    for i in binary.get_content_from_virtual_address(lpValue_ptr, 100):
        if i == 0:
            if flag == 0:
                flag = 1
            else:
                break
        else:
            flag = 0
            namekey += chr(i)

    # Логирование
    print(f"Hooked RegGetValueW: "
          f"hKey={hex(hKey)}, lpSubKey_ptr={hex(lpSubKey_ptr)}, lpValue_ptr={namekey}, dwFlags={dwFlags}")

    global buf_addr
    global length

    if claripy.backends.concrete.convert(state.mem[state.regs.rsp + 0x28].uint64_t.resolved).value not in buf_addr:
        length.append(64)
        buf_addr.append(claripy.backends.concrete.convert(state.mem[state.regs.rsp + 0x28].uint64_t.resolved).value)

    # Возвращаемый результат (ERROR_SUCCESS = 0)
    state.regs.rax = claripy.BVV(0, 32)  # Возвращает 0 (успех)

def hook_reg_set_value_exw(state : angr.SimState):
    hKey = state.solver.eval(state.regs.rcx)  # Первый аргумент (HKEY)
    lpSubKey_ptr = state.solver.eval(state.regs.rdx)  # Второй аргумент (LPCWSTR)
    lpValue_ptr = state.solver.eval(state.regs.r8)  # Третий аргумент
    dwFlags = state.solver.eval(state.regs.r9)  # Четвертый аргумент (флаги)

    # Извлечь дополнительные аргументы из стека
    pdwType_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x28, 8))  # Указатель на тип (REG_*)
    pvData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x30, 8))  # Указатель на буфер данных
    pcbData_ptr = state.solver.eval(state.memory.load(state.regs.rsp + 0x38, 8))  # Указатель на размер данных

    global filename
    namekey = ''
    binary = lief.parse(filename)
    flag = 0
    for i in binary.get_content_from_virtual_address(lpSubKey_ptr, 100):
        if i == 0:
            if flag == 0:
                flag = 1
            else:
                break
        else:
            flag = 0
            namekey += chr(i)

    # Логирование
    print(
        f"Hooked RegSetValueW: "
        f"hKey={hex(hKey)}, lpSubKey_ptr={namekey}, lpValue_ptr={hex(lpValue_ptr)}, "
        f"{dwFlags, pdwType_ptr, hex(pvData_ptr), hex(pcbData_ptr)}")

    # Возвращаемый результат (ERROR_SUCCESS = 0)
    state.regs.rax = claripy.BVV(0, 32)  # Возвращает 0 (успех)

def strcpy_hook(state: angr.SimState):
    dest = state.regs.rcx
    sour = state.regs.r8
    state.regs.rcx = sour
    print(sour, state.regs.rcx)
    print('strcpy')


def hook_getmodulefilenameW(state: angr.SimState):
    name = state.regs.rdx
    size = state.regs.r8
    global length
    global buf_addr
    if claripy.backends.concrete.convert(name).value not in buf_addr:
        length.append(claripy.backends.concrete.convert(size).value)
        buf_addr.append(claripy.backends.concrete.convert(name).value)


def hook_memchr(state: angr.SimState):
    buffer = state.solver.eval(state.regs.rcx)
    symb = state.solver.eval(state.regs.edx)
    size = state.solver.eval(state.regs.r8)
    print(
        f"Hooked memchr: "
        f"buf={hex(buffer)}, char={chr(symb)}, size={size}")
    for i in range(size):
        if symb == state.solver.eval(state.mem[buffer + i * 8].uint8_t.resolved):
            state.regs.rax = claripy.BVV(1, 64)
            return
    state.regs.rax = claripy.BVV(0, 64)
    state.regs.pc = state.mem[state.regs.rsp].uint64_t.resolved
    #state.move(state.solver.eval(state.regs.rsp))


def hook_writefile(state:angr.SimState):
    print('write_file')

def hook_writeconsole(state : angr.SimState):
    print('writeconsole')
    print(state.stack_pop())
    print(state.stack_pop())

def hook_readconsole(state : angr.SimState):
    print('printf', end=' ')
    arg = state.memory.load(state.stack_pop(), 16)
    print(state.solver.eval(arg, cast_to=bytes))
    print(hex(state.addr))