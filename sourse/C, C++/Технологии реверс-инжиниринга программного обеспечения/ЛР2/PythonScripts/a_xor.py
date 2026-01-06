from idaapi import *
import idautils 

def find_xor_primitives():
    for ea in idautils.Heads():
        if idaapi.print_insn_mnem(ea) == "xor":
            print(f"XOR found at {hex(ea)}")
            idc.set_cmt(ea, "XOR  !!!--!!!!!---!!!!!-!!!--!!-!!!!---!!!!", 0)

if __name__ == "__main__":
    find_xor_primitives() 