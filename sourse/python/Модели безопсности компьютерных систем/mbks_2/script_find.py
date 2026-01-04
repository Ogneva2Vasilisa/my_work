import idautils
import idaapi
import idc
import ida_segment

def find_control_transfer_instructions():
    """Находит инструкции для передачи управления в текущей DLL"""
    # Словарь искомых инструкций и их опкодов
    target_instructions = {
        'jmp esp': b'\xFF\xE4',
        'call esp': b'\xFF\xD4',
        'push esp; ret': b'\x54\xC3',
        'jmp eax': b'\xFF\xE0',
        'call eax': b'\xFF\xD0',
        'jmp ebx': b'\xFF\xE3',
        'jmp edx': b'\xFF\xE2',
        'ret': b'\xC3',
        'pop eax; ret': b'\x58\xC3',
        'mov eax, esp; jmp eax': b'\x8B\xC4\xFF\xE0'
    }
    
    print("=== Поиск инструкций передачи управления в DLL ===")
    
    # Проверяем, что файл является DLL
    
    
    results = []
    
    # Перебираем все сегменты
    for seg in idautils.Segments():
        print(seg)
        seg_name = idc.get_segm_name(seg)
        
        # Ищем только в исполняемых сегментах
        if not seg_name.startswith(('.text', 'CODE')):
            continue
            
        seg_start = idc.get_segm_start(seg)
        seg_end = idc.get_segm_end(seg)
        
        print(f"Анализ сегмента (0x{seg_start:08X}-0x{seg_end:08X})")
        
        # Читаем данные сегмента
        seg_data = idaapi.get_bytes(seg_start, seg_end - seg_start)
        if not seg_data:
            continue
        
        # Ищем каждую целевую инструкцию
        for name, opcode in target_instructions.items():
            offset = 0
            while True:
                offset = seg_data.find(opcode, offset)
                if offset == -1:
                    break
                
                ea = seg_start + offset
                results.append((ea, name))
                offset += len(opcode)
    
    # Выводим результаты
    if not results:
        print("Не найдено подходящих инструкций!")
        return
    
    print("\nНайденные инструкции:")
    print("Адрес      | Инструкция")
    print("-----------------------")
    for ea, name in sorted(results):
        print(f"0x{ea:08X} | {name}")
    
    # Сохраняем в файл
    output_file = idaapi.get_root_filename() + "_gadgets.txt"
    with open(output_file, "w") as f:
        f.write("Адрес\tИнструкция\n")
        for ea, name in sorted(results):
            f.write(f"0x{ea:08X}\t{name}\n")
    
    print(f"\nРезультаты сохранены в {output_file}")

if __name__ == "__main__":
    find_control_transfer_instructions()