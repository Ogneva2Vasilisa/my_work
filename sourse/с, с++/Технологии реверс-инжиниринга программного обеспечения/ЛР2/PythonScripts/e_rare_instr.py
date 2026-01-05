def find_rare_instructions():
    rare_instructions = ["int"]  # Add more rare instructions
    rare_argumets = [ "ss", "fs"]  # Add more rare instructions
    for func in idautils.Functions():
        f = ida_funcs.get_func(func)
        for head in idautils.Heads(f.start_ea, f.end_ea):
            mnem = idaapi.print_insn_mnem(head)
            if mnem in rare_instructions:
                print(f"Rare instruction ({mnem}) found at {hex(head)}")
            try:
                for arg in rare_argumets:
                    if idaapi.get_operand_value(head, 0) != narg and arg in idaapi.print_operand(head, 0):
                        print(f"Rare arg ({arg}) found at {hex(head)}")
                    elif idaapi.get_operand_value(head, 1) != narg and arg in idaapi.print_operand(head, 1):
                        print(f"Rare arg ({arg}) found at {hex(head)}")
            except:
                pass
                
if __name__ == "__main__":
    find_rare_instructions()