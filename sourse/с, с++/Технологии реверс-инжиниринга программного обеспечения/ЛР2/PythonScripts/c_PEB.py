from idaapi import *
import idautils
import ida_allins 

def find_peb_reads():
    for func in idautils.Functions():
        f = ida_funcs.get_func(func)
        for head in idautils.Heads(f.start_ea, f.end_ea):
            if idaapi.print_insn_mnem(head) == "mov":
                if "30h" in idaapi.print_operand(head, 1) and "fs" in idaapi.print_operand(head, 1):
                    #print(idaapi.print_operand(head, 1))
                    print(f"PEB read found at {hex(head)}")
                if "60h" in idaapi.print_operand(head, 1) and "gs" in idaapi.print_operand(head, 1):
                    print(f"PEB read found at {hex(head)}")
                
if __name__ == "__main__":
    find_peb_reads() 