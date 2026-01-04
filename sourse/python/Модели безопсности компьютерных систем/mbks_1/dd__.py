import idautils
import idc
import idaapi
import ida_funcs
import ida_name

def find_function_calls(target_functions):
    results = {}
    
    # Перебираем все функции в программе
    for func_ea in idautils.Functions():
        func_name = ida_funcs.get_func_name(func_ea)
        
        # Перебираем все инструкции в функции
        for (start_ea, end_ea) in idautils.Chunks(func_ea):
            for head in idautils.Heads(start_ea, end_ea):
                if idaapi.is_call_insn(head):
                    called_ea = idc.get_operand_value(head, 0)
                    called_name = ida_name.get_name(called_ea)
                    
                    # Проверяем, является ли вызываемая функция целевой
                    if called_name in target_functions:
                        if called_name not in results:
                            results[called_name] = []
                        results[called_name].append(head)
    
    return results

def main():
    # Функции ввода данных
    input_functions = {
        'fread', 'fscanf', 'scanf', 'read', 'fgets', 'gets', 
        'recv', 'recvfrom', 'getc', 'fgetc', 'getchar'
    }
    
    # Небезопасные функции
    unsafe_functions = {
        "_fstrcat", "_fstrcpy", "_fstrncat", "_fstrncpy", "_ftccat", "_ftccpy", "_ftcscat", "_ftcscpy", "_getts", "_gettws", "_getws", "_mbccat", "_makepath", "_mbscat", "_snprintf", "_sntprintf", "_sntscanf", "_snwprintf", "_splitpath", "_stprintf", "_stscanf", "_tccat", "_tccpy", "_tcscat", "_tcscpy", "_tcsncat", "_tcsncpy", "_tmakepath", "_tscanf", "_tsplitpath", "_vsnprintf", "_vsntprintf", "_vsnwprintf", "_vstprintf", "_wmakepath", "_wsplitpath", "OemToCharW", "StrCat", "StrCatA", "StrCatBuff", "StrCatBuffA", "StrCatBuffW", "StrCatChainW", "StrCatN", "StrCatNA", "StrCatNW", "StrCatW", "StrCpy", "StrCpyA", "StrCpyN", "StrCpyNA", "StrCpyNW", "strcpyW", "StrCpyW", "StrNCat", "StrNCatA", "StrNCatW", "StrNCpy", "StrNCpyA", "StrNCpyW", "gets", "lstrcat", "lstrcatA", "lstrcatn", "lstrcatnA", "lstrcatnW", "lstrcatW", "lstrcpy", "lstrcpyA", "lstrcpyn", "lstrcpynA", "lstrcpynW", "lstrcpyW", "snscanf", "snwscanf", "sprintf", "sprintfA", "sprintfW", "lstrncat", "makepath", "nsprintf", "strcat", "strcatA", "strcatW", "strcpy", "strcpyA", "strncat", "strncpy", "swprintf", "ualstrcpyW", "vsnprintf", "vsprintf", "vswprintf", "wcscat", "wcscpy", "wcsncat", "wcsncpy", "wnsprintf", "wnsprintfA", "wsprintf", "wsprintfA", "wsprintfW", "wvnsprintf", "wvnsprintfA", "wvnsprintfW", "wvsprintf", "wvsprintfA", "wvsprintfW"
        'strcpy', 'sprintf', 'strncpy', 'memcpy', 'memmove',
        'strcat', 'strncat', 'vsprintf', 'wcscpy', 'wcsncpy'
    }
    
    print("=== Поиск функций ввода данных ===")
    input_results = find_function_calls(input_functions)
    for func_name, call_addrs in input_results.items():
        print(f"\nФункция: {func_name}")
        for addr in call_addrs:
            print(f"  Вызов из: 0x{addr:X}")
    
    print("\n=== Поиск небезопасных функций ===")
    unsafe_results = find_function_calls(unsafe_functions)
    for func_name, call_addrs in unsafe_results.items():
        print(f"\nФункция: {func_name}")
        for addr in call_addrs:
            print(f"  Вызов из: 0x{addr:X}")

if __name__ == "__main__":
    main()