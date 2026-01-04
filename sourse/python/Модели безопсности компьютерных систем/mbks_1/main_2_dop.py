import os
import subprocess
import time
import shutil
from collections import defaultdict
from windbg_2_dop import *
from my_class_dop import *



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

def run_dynamorio_analysis(log_dir, modifications=None):
    """Запуск анализа с DynamoRIO"""
    try:
        # Создаем команду для запуска Excel через DynamoRIO
        cmd = [
            drrun_path,
            "-c", "drcov.dll",
            "--", EXCEL_PATH,
            TARGET_FILE
        ]
        
        # Запускаем процесс
        process = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        stdout, stderr = process.communicate()
        
        # Сохраняем логи
        with open(os.path.join(log_dir, "dynamorio_stdout.log"), "wb") as f:
            f.write(stdout)
        with open(os.path.join(log_dir, "dynamorio_stderr.log"), "wb") as f:
            f.write(stderr)
            
        return process.returncode
    except Exception as e:
        print(f"Ошибка при запуске DynamoRIO: {str(e)}")
        return -1

# Модифицированный брутфорс
def brute_force_analysis(max_iterations=None):
     # Создаем резервную копию
    if not os.path.exists(BACKUP_FILE):
        shutil.copyfile(TARGET_FILE, BACKUP_FILE)
    
    with open(BACKUP_FILE, 'rb') as f:
        original_data = bytearray(f.read())

    total_bytes = 0x40
    #total_bytes = len(original_data)
    error_counter = defaultdict(int)
    iteration_count = 0
    
    try:
        for offset in range(total_bytes):
            consecutive_errors = 0
            last_error = None
            for new_byte in [0x00, 0xFF]:
                if original_data[offset] == new_byte:
                    continue

                # Проверяем ограничение итераций
                if max_iterations is not None and iteration_count >= max_iterations:
                    print(f"\n▓ Достигнуто максимальное количество итераций ({max_iterations})")
                    return

                iteration_count += 1

                # Восстанавливаем оригинал из резервной копии
                shutil.copyfile(BACKUP_FILE, TARGET_FILE)
                
                # Модифицируем файл
                with open(TARGET_FILE, 'r+b') as f:
                    f.seek(offset)
                    f.write(bytes([new_byte]))
                
                log_dir = f"brute_logs/offset_{offset:04X}_val_{new_byte:02X}_"
                log_filename = f"drcov_{time.ctime()}.log"
                full_log_path = os.path.join(log_dir, log_filename)
                os.makedirs(log_dir, exist_ok=True)

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
        shutil.copyfile(BACKUP_FILE, TARGET_FILE)
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
                replace_bytes(TARGET_FILE, {offset: value})
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
                replace_bytes(TARGET_FILE, mods)
            except ValueError:
                print("Некорректный ввод!")

        elif choice == '3':
            data = bytes.fromhex(input("Данные для дописи (hex): "))
            append_to_file(TARGET_FILE, data)

        elif choice == '4':
            print("Найденные разделители:")
            for sep, positions in find_separators(TARGET_FILE).items():
                print(f"{sep.decode()} ({len(positions)}): {[str(hex(pos)) for pos in positions]}...")

        elif choice == '5':
            try:
                start = int(input("Начало поля (hex): "), 16)
                end = int(input("Конец поля (hex): "), 16)
                data = bytes.fromhex(input("Данные для дописи (hex): "))
                expand_field(TARGET_FILE, (start, end), data)
            except ValueError:
                print("Некорректный ввод!")

        elif choice == '6':
            try:
                iterations = input("Введите количество итераций (пусто для без ограничений): ")
                max_iterations = int(iterations) if iterations else None
                if max_iterations:
                    print(f"▓ Запуск брутфорса на {max_iterations} итераций")
                else:
                    print("▓ Запуск брутфорса без ограничений")
                brute_force_analysis(max_iterations)
            except ValueError:
                print("Некорректный ввод! Запуск без ограничений")
                brute_force_analysis()

        elif choice == '7':
            try:
                iterations = input("Введите количество итераций (по умолчанию 100): ")
                max_iterations = int(iterations) if iterations else 100
                print(f"▓ Запуск фаззинга на {max_iterations} итераций")
                fuzzer = CoverageGuidedFuzzer(max_iterations=max_iterations)
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