def find_other_protection_signs():
    for func in idautils.Functions():
        f = ida_funcs.get_func(func)
        for head in idautils.Heads(f.start_ea, f.end_ea):
            if "" in idaapi.print_operand(head, 0):
                print(f"Possible other protection at {hex(head)}")

if __name__ == "__main__":
    find_other_protection_signs()