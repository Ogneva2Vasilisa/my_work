import os
import subprocess
import time
import shutil
from collections import defaultdict
from windbg_2 import *
from my_class import *

# Конфигурация
SEPARATORS = list(b",:=;%./\"")

ERROR_LOG = "error_log.json"
MAX_CONSECUTIVE_ERRORS = 5
BACKUP_FILE = "config_15.original"
drrun_path = r"C:\Users\Vasilisa\Downloads\DynamoRIO-Windows-11.90.20168\bin32\drrun.exe"


# Модифицированные функции работы с файлами
def replace_bytes(filename, modifications):
    """
    Замена байтов в файле
    modifications = {offset: new_byte}
    """
    try:
        with open(filename, 'r+b') as f:
            data = bytearray(f.read())
            for offset, new_byte in modifications.items():
                if offset < len(data):
                    data[offset] = new_byte
            f.seek(0)
            f.write(data)
        return True
    except Exception as e:
        print(f"Ошибка замены: {str(e)}")
        return False

def append_to_file(filename, data):
    """Дозапись данных в конец файла"""
    try:
        with open(filename, 'ab') as f:
            f.write(data)
        return True
    except Exception as e:
        print(f"Ошибка дозаписи: {str(e)}")
        return False

def find_separators(filename):
    """Поиск позиций разделителей в файле"""
    separators = defaultdict(list)
    try:
        with open(filename, 'rb') as f:
            data = f.read()
            for i, byte in enumerate(data):
                if byte in SEPARATORS:
                    separators[bytes([byte])].append(i)
        return separators
    except Exception as e:
        print(f"Ошибка поиска разделителей: {str(e)}")
        return None

def expand_field(filename, field_pos, append_data):
    """
    Расширение поля
    field_pos - (start, end)
    append_data - данные для дописывания
    """
    try:
        with open(filename, 'r+b') as f:
            data = bytearray(f.read())
            new_data = data[:field_pos[1]] + append_data + data[field_pos[1]:]
            f.seek(0)
            f.write(new_data)
        return True
    except Exception as e:
        print(f"Ошибка расширения поля: {str(e)}")
        return False

# Модифицированный брутфорс
def brute_force_analysis():
     # Создаем резервную копию
    if not os.path.exists(BACKUP_FILE):
        shutil.copyfile("config_15", BACKUP_FILE)
    
    with open(BACKUP_FILE, 'rb') as f:
        original_data = bytearray(f.read())

    total_bytes = 0x40
    #total_bytes = len(original_data)
    error_counter = defaultdict(int)
    
    try:
        for offset in range(total_bytes):
            consecutive_errors = 0
            last_error = None
            #for new_byte in [0x00, 0xFF, 0xFF//2, 0xFF//2+1]:
            for new_byte in [0x00, 0xFF]:
                if original_data[offset] == new_byte:
                    continue

                # Восстанавливаем оригинал из резервной копии
                shutil.copyfile(BACKUP_FILE, "config_15")
                
                # Модифицируем файл
                with open("config_15", 'r+b') as f:
                    f.seek(offset)
                    f.write(bytes([new_byte]))
                
                
                log_dir = f"brute_logs/offset_{offset:04X}_val_{new_byte:02X}_"
                 # Генерируем уникальное имя для лог-файла
                log_filename = f"drcov_{time.ctime()}.log"
                full_log_path = os.path.join(log_dir, log_filename)
                os.makedirs(log_dir, exist_ok=True)
                #shutil.copy("e://mbks_1/config_15","log_dir")

                # Запускаем анализ
                error_code = run_dynamorio_analysis(log_dir, {offset: new_byte})
                run_windbg_coverage(log_dir)
                # Обработка ошибок
                if error_code != 0:
                    error_counter[error_code] += 1
                    if error_code == last_error:
                        consecutive_errors += 1
                        if consecutive_errors >= MAX_CONSECUTIVE_ERRORS:
                            print(f"▓ Обнаружено {MAX_CONSECUTIVE_ERRORS} ошибок. Пропуск смещения {offset:04X}")
                            break
                    else:
                        consecutive_errors = 1
                        last_error = error_code
                else:
                    consecutive_errors = 0
                    last_error = None

                print(f"[{offset:04X}/{total_bytes:04X}] {new_byte:02X} | Ошибка: {error_code}")

    finally:
        # Восстанавливаем оригинальный файл
        shutil.copyfile(BACKUP_FILE, "config_15")
        print("\nОригинальный файл восстановлен")

    print("\nСтатистика ошибок:")
    for code, count in error_counter.items():
        print(f"       {count} ошибок")

def main():
    drrun_path = setup_dynamorio()
    
    if not drrun_path:
        return False
    
    while True:
        print("\nМеню:")
        print("0 - Анализ")
        print("1 - Однобайтовая замена")
        print("2 - Многобайтовая замена")
        print("3 - Дозапись в файл")
        print("4 - Поиск разделителей")
        print("5 - Расширение поля")
        print("6 - Запуск брутфорса")
        print("7 - Умный фаззинг на основе покрытия")
        print("q - Выход")

        choice = input("> ").lower()
        if choice == '0':
            manual_log_dir = f"manual_analysis_logs_{int(time.time())}"
            os.makedirs(manual_log_dir, exist_ok=True)
            run_dynamorio_analysis_0(manual_log_dir)

        elif choice == '1':
            try:
                offset = int(input("Смещение (hex): "), 16)
                value = int(input("Значение (hex): "), 16)
                replace_bytes("config_15", {offset: value})
            except ValueError:
                print("Некорректный ввод!")

        elif choice == '2':
            try:
                mods = {}
                while True:
                    offset = input("Смещение (hex, пусто для завершения): ")
                    if not offset: break
                    value = input("Значение (hex): ")
                    mods[int(offset, 16)] = int(value, 16)
                replace_bytes("config_15", mods)
            except ValueError:
                print("Некорректный ввод!")

        elif choice == '3':
            data = bytes.fromhex(input("Данные для дописи (hex): "))
            append_to_file("config_15", data)

        elif choice == '4':
            print("Найденные разделители:")
            for sep, positions in find_separators("config_15").items():
                print(f"{sep.decode()} ({len(positions)}): {[str(hex(pos)) for pos in positions]}...")

        elif choice == '5':
            try:
                start = int(input("Начало поля (hex): "), 16)
                end = int(input("Конец поля (hex): "), 16)
                data = bytes.fromhex(input("Данные для дописи (hex): "))
                expand_field("config_15", (start, end), data)
            except ValueError:
                print("Некорректный ввод!")

        elif choice == '6':
            brute_force_analysis()

        elif choice == '7':
            try:
                iterations = input("Введите количество итераций (по умолчанию 100): ")
                max_iterations = int(iterations) if iterations else 100
                print(f"▓ Запуск фаззинга на {max_iterations} итераций")
                fuzzer = CoverageGuidedFuzzer(max_iterations=max_iterations)
                fuzzer.calculate_total_addresses("initial_analysis")
                fuzzer.smart_fuzz()
            except ValueError:
                print("Некорректный ввод! Используется значение по умолчанию (100)")
                fuzzer = CoverageGuidedFuzzer()
                fuzzer.smart_fuzz()

        elif choice == 'q':
            # Закрываем JSON файл
            with open(ERROR_LOG, 'a', encoding='utf-8') as f:
                f.write('\n]')  # Закрывающая квадратная скобка для JSON массива
            break

if __name__ == "__main__":
    main()