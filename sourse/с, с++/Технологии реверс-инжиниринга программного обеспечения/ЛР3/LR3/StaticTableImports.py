from hooks import *
from Bin_updating import *
import sys


sources = {'gets_s': [hook_gets, 6], 'ReadFile' : [readfile_hook, 6], 'RegOpenKeyExW' : [hook_reg_open_key_exw, 6], \
            'RegQueryValueExW': [hook_reg_query_value_exw, 6], 'RegGetValueW' : [hook_reg_get_value_w, 6], \
           'RegSetValueExW': [hook_reg_set_value_exw, 6], 'GetModuleFileNameW' : [hook_getmodulefilenameW, 6], \
           'fgets': [hook_gets, 6]}

sink = ['puts', 'WriteFile', 'RegSetValueExW', 'SetPriorityClass', 'FindFirstFileW', 'CloseHandle', 'fputs']
hooks = {'strcpy_s': [strcpy_hook, 6], 'WriteFile': [hook_writefile, 6], 'memchr' : [hook_memchr, 6], 'WriteConsoleW': [hook_writeconsole, 6], 'ReadConsoleW' : [hook_readconsole, 6]}
patch_hooks = {'IsDebuggerPresent': 6}



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

def SearchFunc(p: angr.Project, funcaddr: str):
    cfg = p.analyses.CFGFast()
    cfg.normalize()
    arguments = list()
    argument = None
    for func_node in cfg.kb.functions.values():
        for block in func_node.blocks:
            #block.pp()
            addr = block.addr - 1 if block.thumb else block.addr
            ins_addr = list(block._project.analyses.Disassembly(ranges=[(addr, addr + block.size)], thumb=block.thumb,
                                                                block_bytes=block.bytes).raw_result_map["instructions"])
            for i in range(len(ins_addr)):
                a = str(list(block._project.analyses.Disassembly(ranges=[(addr, addr + block.size)],
                                                                 thumb=block.thumb,
                                                                 block_bytes=block.bytes).raw_result_map["instructions"].values())[i].render())
                if 'push' in a:
                    argument = a
                if funcaddr in a:
                    print(hex(list(block._project.analyses.Disassembly(ranges=[(addr, addr + block.size)],
                                                                 thumb=block.thumb,
                                                                 block_bytes=block.bytes).raw_result_map["instructions"].keys())[i]))
                    arguments.append(argument.split(' ')[4].split("'")[0])
                    break
    return arguments

def FindArgs(funcname:str, call_addresses:dict, proj:angr.Project):
    lib = proj.loader.main_object.imports
    addr = lib[funcname]
    cfg = proj.analyses.CFGFast()
    cfg.normalize()
    arguments = list()
    #binary = lief.parse("test1.exe")
    for key, value in call_addresses.items():
        if value == hex(addr.rebased_addr):
            for func_node in cfg.functions.values():
                if ('print' in funcname or 'scanf' in funcname) and key > func_node.addr and key < func_node.addr + func_node.size:
                    print(hex(func_node.addr))
                    arguments = SearchFunc(proj, func_node.name)
                else:
                    for block in func_node.blocks:
                        if key >= block.addr and key <= block.addr + block.size:
                            insns = block._project.analyses.Disassembly(ranges=[(block.addr, block.addr + block.size)], thumb=block.thumb,block_bytes=block.bytes).raw_result_map["instructions"]
                            for i in range(len(insns.values())-1, -1, -1):
                                if (funcname == 'puts' or funcname == 'gets_s') and 'rcx' in str(list(insns.values())[i].render()) or (funcname == 'WriteFile' or funcname == 'ReadFile') and 'rdx' in str(list(insns.values())[i].render()):
                                    arguments.append(str(list(insns.values())[i].render()).split('[')[2].split(']')[0])
    return arguments


def find_put_args(addr, proj: angr.Project):
    cfg = proj.analyses.CFGFast()
    addrs = []
    for func_node in cfg.kb.functions.values():
        for block in func_node.blocks:
            if addr >= block.addr and addr <= block.addr + block.size:
                insns = \
                block._project.analyses.Disassembly(ranges=[(block.addr, block.addr + block.size)], thumb=block.thumb,
                                                    block_bytes=block.bytes).raw_result_map["instructions"]
                for key, value in insns.items():
                    if 'rcx' in str(value.render()):
                        addrs.append(key)
            else:
                insns = \
                block._project.analyses.Disassembly(ranges=[(block.addr, block.addr + block.size)], thumb=block.thumb,
                                                    block_bytes=block.bytes).raw_result_map["instructions"]
                for key, value in insns.items():
                    if str(hex(addr)) in str(value.render()) and 'jmp' in str(value.render()):
                        addrs.append(block.addr)
    return addrs

def getaddrsource(proj: angr.Project, sourcefunc: int, len: int):
    initial_state = proj.factory.call_state(0x140001000)
    #initial_state = proj.factory.entry_state()
    initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_REGISTERS)
    initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_MEMORY)
    simulation = proj.factory.simgr(initial_state)

    @proj.hook(sourcefunc, length=len)
    def ok(state: angr.SimState):
        #if proj.arch == 'x86_64':
        print(state.mem[state.regs.rcx])
        print(state.mem[state.regs.rdx])
        print(state.regs.rdx)
        print(state.regs.edx)
        print(state.regs.eax)
        print(state.regs.r8d)
        print('5 аргумент -', state.mem[state.regs.rsp + 0x28].uint64_t)
        proj.terminate_execution()

    simulation.run()


def Search(project: angr.Project, findaddr:int, sourceaddr: int, len: int):
    # Start in main()
    #initial_state = project.factory.entry_state()
    initial_state = project.factory.call_state(0x140001000)
    initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_REGISTERS)
    initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_MEMORY)
    symb_vector = claripy.BVS('input', len * 8)
    initial_state.memory.store(sourceaddr, symb_vector)
    simulation = project.factory.simgr(initial_state)
    # Find the way yo reach the good address
    good_address = findaddr

    simulation.explore(find=good_address)

    # If found a way to reach the address
    if simulation.found:
        solution_state = simulation.found[0]
        win_sequence = ''
        finishedTracing = False
        for win_block in solution_state.history.bbl_addrs.hardcopy:
            win_block = project.factory.block(win_block)
            addresses = win_block.instruction_addrs
            for address in addresses:
                win_sequence += 't:' + hex(address) + '\n'
                if address == good_address:
                    # Prevent sending the rest of the block addresses that aren't desired
                    finishedTracing = True
                    break
            if finishedTracing:
                break
        win_sequence = win_sequence[:-1]
        #print(win_sequence)
        # Print the string that Angr wrote to stdin to follow solution_state
        #print(solution_state.posix.dumps(1))
        print(solution_state.solver.eval(symb_vector, cast_to=bytes))
        #print(solution_state.posix.dumps(sys.stdin.fileno()))
    else:
        print('Could not find the solution')


def init_hook_adr_and_pathing(proj, call_imports, lib):
    address_names = dict()

    for key, value in call_imports.items():
        if value == 'sub_401050':
            proj.hook(key, hook_readconsole, 5)
            sink_addr.append(key)
        elif value == 'sub_4010c0':
            proj.hook(key, hook_writeconsole, 5)
        else:
            index = lib.index(hex(int(value, 0)))
            func_name_imported = list(proj.loader.main_object.imports.keys())[index]
            print(f"  0x{key:x} in lib", func_name_imported)
            address_names.update({key: func_name_imported})
            if func_name_imported in sources.keys():
                hook_addr.append(key)
                proj.hook(key, sources[func_name_imported][0], sources[func_name_imported][1])
            if func_name_imported in sink:
                if func_name_imported == 'puts':
                    addrs = find_put_args(key, proj)
                    for i in range(len(addrs)):
                        sink_addr.append(addrs[i])
                else:
                    sink_addr.append(key)
            if func_name_imported in hooks.keys():
                hook_addr.append(key)
                proj.hook(key, hooks[func_name_imported][0], hooks[func_name_imported][1])

            if func_name_imported in patch_hooks.keys():
                patch_nop(key, patch_hooks[func_name_imported], proj.kb.patches)
    return address_names