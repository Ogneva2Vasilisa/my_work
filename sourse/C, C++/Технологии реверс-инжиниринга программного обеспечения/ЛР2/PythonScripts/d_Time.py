
def Time_addr():
    for ea in idautils.Heads():
        if idaapi.print_insn_mnem(ea) == "rdtsc":
            print(f"Time_program  rdtsc  found at {hex(ea)}")
            #idc.set_cmt(ea, "!!!!!!Time count!!!!", 0)
        if (idaapi.print_insn_mnem(ea) == "call" or idaapi.print_insn_mnem(ea) == "invoke") and "GetTickCount" in idaapi.print_operand(ea, 0):
            print(f"Time_program  GetTickCount  found at {hex(ea)}")

if __name__ == "__main__":
    Time_addr() 