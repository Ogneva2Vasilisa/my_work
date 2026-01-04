from idaapi import *
from idc import *
from idautils import *

class FuncCoverage(DBG_Hooks):
    # Our breakpoint handler
    def dbg_bpt(self, tid, ea):
        print(f"[*] Hit: {ea:.8f}")
        return

debugger = FuncCoverage()
debugger.hook()

current_addr = idc.get_screen_ea()

# Find all functions and add breakpoints
for func in idautils.Functions():
    f = ida_funcs.get_func(func)
    for head in idautils.Heads(f.start_ea, f.end_ea):
        idc.add_bpt(f.start_ea)  # Используем idc.add_bpt вместо AddBpt
        idc.set_bpt_attr(f.start_ea, BPTATTR_FLAGS, 0x0)

num_breakpoints = idc.get_bpt_qty()  # Используем idc.get_bpt_qty
print(f"[*] Set {num_breakpoints} breakpoints.")

print(f"[*] Hit: {num_breakpoints} breakpoints.")