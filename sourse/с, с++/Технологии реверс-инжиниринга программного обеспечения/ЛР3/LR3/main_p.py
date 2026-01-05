from my_funcs import *


def main (path):
    proj = angr.Project(path, auto_load_libs=False)
    cfg = proj.analyses.CFGFast(resolve_indirect_jumps=True)

    init_sink_source(proj)
    create_dis_asm(proj,cfg)

    for node in cfg.model.nodes():
        block = proj.factory.block(node.addr)
        insns= block.capstone.insns
        for i in range(len(insns)):
            insn=insns[i]
            if insn.mnemonic == "call":
                call_addr = insn.address
                call_site = node.addr
                target = call_addr+6+insn.disp
                for sinks_f in sinks_funcs:
                    if target == sinks_f[1]:
                        func_name = sinks_f[0]
                        print(f"Call at 'block' {hex(call_site)} -> {func_name}, called in {hex(call_addr)}")

                        # Анализ аргументов
                        if func_name=='puts':
                            j=0
                            while True:
                                if insns[i-j].mnemonic=="lea" and insns[i-j].op_str[:4]=="rcx,":
                                    str_addr=insns[i-j].address+insns[i-j].disp+7
                                    # print(hex(insns[i-j].address), insns[i-j].op_str, 'string at ', hex(str_addr))
                                    read_str_from_addr(proj, str_addr)
                                    break
                                j=j+1
                        if func_name=='WriteFile':
                            j=0
                            while True:
                                if insns[i-j].mnemonic=="lea" and insns[i-j].op_str[:4]=="rdx,":
                                    str_addr=insns[i-j].address+insns[i-j].disp+7
                                    # print(hex(insns[i-j].address), insns[i-j].op_str, 'string at ', hex(str_addr))
                                    read_str_from_addr(proj, str_addr)
                                    break
                                j=j+1
                for source_f in source_funcs:
                    if target == source_f[1]:
                        func_name = source_f[0]
                        print(f"Call at 'block' {hex(call_site)} -> {func_name}, called in {hex(call_addr)}")

                        # Анализ аргументов
                        if func_name=='gets_s':
                            j=0
                            while True:
                                if insns[i-j].mnemonic=="lea" and insns[i-j].op_str[:4]=="rcx,":
                                    str_addr=insns[i-j].address+insns[i-j].disp+7
                                    print(hex(insns[i-j].address), insns[i-j].op_str, 'Buffer at ', insns[i-j].op_str[5:])
                                    # read_str_from_addr(proj, str_addr)
                                    break
                                j=j+1
                        if func_name=='ReadFile':
                            j=0
                            while True:
                                if insns[i-j].mnemonic=="lea" and insns[i-j].op_str[:4]=="rdx,":
                                    str_addr=insns[i-j].address+insns[i-j].disp+7
                                    print(hex(insns[i-j].address), insns[i-j].op_str, 'Buffer at ', insns[i-j].op_str[5:])
                                    # read_str_from_addr(proj, str_addr)
                                    break
                                j=j+1

main('test1.exe')