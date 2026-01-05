from StaticTableImports import *

# Загрузка файла
proj = angr.Project(filename, load_options={'auto_load_libs': False})
lib = [hex(x.rebased_addr) for x in proj.loader.main_object.imports.values()]  # Загрузка таблицы импортов


call_imports = GetStaticImportAdderess(proj) # Статическая загрузка всех адресов, где вызываются импортные функции
address_names = init_hook_adr_and_pathing(proj,call_imports, lib)
bin_save('output.exe', proj.kb.patches)
print(f'{filename} patched in output.exe')

print(call_imports)
#print(address_names)

initial_state = proj.factory.call_state(0x140001000)
#initial_state = proj.factory.entry_state()

initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_REGISTERS)
initial_state.options.add(angr.options.SYMBOL_FILL_UNCONSTRAINED_MEMORY)
simulation = proj.factory.simgr(initial_state)

simulation.run()
print('Simvectors ready!\n')

print(sink_addr)
print(buf_addr)

for i in range(len(sink_addr)):
    for j in range(len(buf_addr)):
        symb_vector = claripy.BVS('input', length[j] * 8)
        initial_state.memory.store(buf_addr[j], symb_vector)

        simulation = proj.factory.simgr(initial_state)

        simulation.explore(find=sink_addr[i])

        if simulation.found:
            solution_state = simulation.found[0]
            win_sequence = ''
            finishedTracing = False
            for win_block in solution_state.history.bbl_addrs.hardcopy:
                win_block = proj.factory.block(win_block)
                addresses = win_block.instruction_addrs
                for address in addresses:
                    win_sequence += 't:' + hex(address) + '\n'
                    if address == sink_addr[i]:
                        finishedTracing = True
                        break
                if finishedTracing:
                    break
            win_sequence = win_sequence[:-1]
            #print(win_sequence)
            if buf_addr != 0:
                print(hex(sink_addr[i]), solution_state.solver.eval(symb_vector, cast_to=bytes))
        else:
            print(hex(sink_addr[i]), 'Could not find the solution or solution in func arguments')
        print()