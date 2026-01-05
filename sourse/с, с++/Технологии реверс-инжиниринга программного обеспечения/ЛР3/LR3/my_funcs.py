import angr

sinks_funcs=[]
source_funcs=[]
sinks_addrs=[]
source_addrs=[]
insns1 =[]

def create_dis_asm(proj, cfg):
    for node in cfg.model.nodes():
        block = proj.factory.block(node.addr)
        insns = block.capstone.insns
        for i in range(len(insns)):
            insn = insns[i]
            insns1.append([insns[i].address, insns[i].mnemonic+' '+insns[i].op_str, insns[i]])

def init_sink_source(proj):
    for func_name, reloc in proj.loader.main_object.imports.items():
        # print(f"Функция: {func_name}")
        # print(f"Адрес (PLT): {hex(reloc.rebased_addr)}")
        # print(f"Символ: {reloc.resolvedby}")
        if (func_name == 'puts' or func_name == 'printf' or func_name == 'WriteFile'):
            sinks_addrs.append(reloc.rebased_addr)
            sinks_funcs.append([func_name, reloc.rebased_addr, reloc.resolvedby])
        if (func_name == 'gets' or func_name == 'getc' or func_name == 'gets_s' or func_name == 'ReadFile'):
            source_addrs.append(reloc.rebased_addr)
            source_funcs.append([func_name, reloc.rebased_addr, reloc.resolvedby])


def read_str_from_addr(proj, str_addr):
    # Адрес, откуда нужно прочитать строку (например, str_addr = 0x140002250)
    # Прочитать данные как байты
    data = proj.loader.memory.load(str_addr, 50)  # 50 байт
    # Преобразовать в строку (до первого нулевого байта)
    null_index = data.find(b"\x00")
    if null_index != -1:
        data = data[:null_index]
    concrete_string = data.decode("utf-8", errors="ignore")
    print(f"    Строка по адресу {hex(str_addr)}: {concrete_string}")



