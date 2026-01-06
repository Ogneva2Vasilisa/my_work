from idaapi import *
import idautils
import ida_allins 

def find_xor_encoded_strings():
    

    for ea in Functions():
        f = ida_funcs.get_func(ea)
        #print(ida_funcs.get_func_name(ea)) # not necessarily the start ea
        #print("%x %x" % (f.start_ea, f.end_ea)) #start_ea = %x" % ea
        for ea_f in Heads(f.start_ea, f.end_ea):
            insn = idaapi.insn_t()
            length = idaapi.decode_insn(insn, ea_f)
            if insn.itype != ida_allins.NN_xor:
                continue
            if insn.ops[1].type == ida_ua.o_imm:
                print("Data is xor with const %x at addr %x" % (insn.ops[1].value, ea_f))
        
      

if __name__ == "__main__":
    find_xor_encoded_strings() 