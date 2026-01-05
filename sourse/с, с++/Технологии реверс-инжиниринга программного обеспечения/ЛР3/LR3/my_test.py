import claripy
from my_funcs import *
length = []
buf_addr = []

hook_addr = []
def GetStaticImportAdderess(p : angr.Project):
    lib = [hex(x.rebased_addr) for x in p.loader.main_object.imports.values()]
    call_addresses = {}
    cfg = p.analyses.CFGFast()
    #cfg = p.analyses.CFGEmulated()
    cfg.normalize()
    for func_node in cfg.kb.functions.values():
        for block in func_node.blocks:
            addr = block.addr - 1 if block.thumb else block.addr
            ins_addr = list(block._project.analyses.Disassembly(ranges=[(addr, addr + block.size)], thumb=block.thumb,
                                                                block_bytes=block.bytes).raw_result_map["instructions"])
            if len(ins_addr) > 0:
                ins_addr = ins_addr[-1]
                a = str(list(block._project.analyses.Disassembly(ranges=[(addr, addr + block.size)], thumb=block.thumb,
                                                                 block_bytes=block.bytes).raw_result_map[
                                 "instructions"].values())[-1].render())
                for i in lib:
                    if i in a:
                        call_addresses.__setitem__(ins_addr, i)
                        break
    return call_addresses

def hook_gets(state: angr.SimState):
    global length
    global buf_addr
    size = state.regs.rdx
    buffer = state.regs.rcx
    if claripy.backends.concrete.convert(buffer).value not in buf_addr:
        length.append(claripy.backends.concrete.convert(size).value)
        buf_addr.append(claripy.backends.concrete.convert(buffer).value)

proj = angr.Project("test1.exe", auto_load_libs=False)

sources = {'gets_s': [hook_gets, 6]}
lib = [hex(x.rebased_addr) for x in proj.loader.main_object.imports.values()]  # Загрузка таблицы импортов
address_names = dict()



call_imports = GetStaticImportAdderess(proj)
for key, value in call_imports.items():
    index = lib.index(hex(int(value, 0)))
    print(f"  0x{key:x} in lib", list(proj.loader.main_object.imports.keys())[index])
    address_names.update({key: list(proj.loader.main_object.imports.keys())[index]})
    if list(proj.loader.main_object.imports.keys())[index] in sources.keys():
        hook_addr.append(key)
        proj.hook(key, sources[list(proj.loader.main_object.imports.keys())[index]][0],
                  sources[list(proj.loader.main_object.imports.keys())[index]][1])

    if list(proj.loader.main_object.imports.keys())[index] in hooks.keys():
        hook_addr.append(key)
        proj.hook(key, hooks[list(proj.loader.main_object.imports.keys())[index]][0],
                  hooks[list(proj.loader.main_object.imports.keys())[index]][1])




# Загрузить бинарный файл


init_sink_source(proj)

# Создать символьную переменную для входных данных
input_size = 16  # 16 байт
length =[15]

for i in range(len(sinks_addrs)):
    for j in range(len(buf_addr)):
        sym_input = claripy.BVS("input", 15 * 8)

        # Создать начальное состояние с символьными данными
        state = proj.factory.entry_state(stdin=sym_input, buf_addr=buf_addr[j])

        # Запустить символьное выполнение
        simgr = proj.factory.simulation_manager(state)
        simgr.explore(find=sinks_addrs[i])  # Адрес целевой точки

        # Получить конкретные входные данные
        if simgr.found:
            found_state = simgr.found[0]
            concrete_input = found_state.solver.eval(sym_input, cast_to=bytes)
            print(f"Входные данные для достижения целевой точки: {concrete_input}")