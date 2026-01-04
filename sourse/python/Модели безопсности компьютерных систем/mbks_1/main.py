import os
import subprocess
import shutil
import time

def setup_dynamorio():
    """Проверка наличия DynamoRIO и клиентских инструментов"""
    dynamorio_home = r"C:\Users\Vasilisa\Downloads\DynamoRIO-Windows-11.90.20168"
    
    drrun = os.path.join(dynamorio_home, "bin32", "drrun.exe")
    drcov = os.path.join(dynamorio_home, "tools", "bin32", "drcov2text.exe")
    
    return (drrun if os.path.exists(drrun) else None, 
            drcov if os.path.exists(drcov) else None)

def run_dynamorio_analysis(log_dir):
    """Запуск анализа с DynamoRIO и сохранение результатов в указанную папку"""
    try:
        drrun_path, drcov_path = setup_dynamorio()
        if not drrun_path:
            print("DynamoRIO не найден! Установите и задайте DYNAMORIO_HOME")
            return False

        if not os.path.exists("vuln15.exe"):
            print("Ошибка: vuln15.exe не найден!")
            return False

        os.makedirs(log_dir, exist_ok=True)

        # 1. Собираем покрытие кода
        coverage_cmd = [
            drrun_path,
            "-t", "drcov",
            "-dump_text",
            "-logdir", log_dir,
            "--", "vuln15.exe"
        ]

        # 2. Собираем регистры и стек (используем клиентский инструмент drstrace)
        trace_cmd = [
            drrun_path,
            "-t", "drstrace",
            "-logdir", log_dir,
            "--", "vuln15.exe"
        ]

        print("[+] Запуск DynamoRIO (анализ покрытия)...")
        subprocess.run(coverage_cmd, check=True)

        print("[+] Запуск DynamoRIO (трассировка регистров/стека)...")
        subprocess.run(trace_cmd, check=True)

        # Обработка результатов
        print("\n[+] Результаты:")
        for fname in os.listdir(log_dir):
            if fname.startswith("drcov.vuln15.exe"):
                log_file = os.path.join(log_dir, fname)
                print(f"[+] Файл покрытия: {log_file}")
                return True
        
        print("[-] Файл покрытия не создан")
        return False

    except subprocess.CalledProcessError as e:
        print(f"Ошибка DynamoRIO: {e}")
        return False

def replace_byte_in_file(filename, offset, new_byte):
    """Замена байта в файле"""
    try:
        with open(filename, 'rb') as f:
            data = bytearray(f.read())
        
        if offset >= len(data):
            print(f"Ошибка: смещение {offset} превышает размер файла")
            return False

        old_byte = data[offset]
        data[offset] = new_byte
        
        with open(filename, 'wb') as f:
            f.write(data)
        
        print(f"Замена: 0x{old_byte:02X} → 0x{new_byte:02X} по смещению {offset}")
        return True

    except Exception as e:
        print(f"Ошибка замены: {str(e)}")
        return False

def brute_force_replace_byte(filename):
    """Перебор значений байта (00 и FF) в файле для всех смещений с анализом после каждой замены"""
    brute_force_log_dir = "brute_force_logs"
    os.makedirs(brute_force_log_dir, exist_ok=True)

    try:
        with open(filename, 'rb') as f:
            data = bytearray(f.read())
        
        for offset in range(len(data)):
            for new_byte in [0x00, 0xFF]:
                if replace_byte_in_file(filename, offset, new_byte):
                    print(f"Запуск анализа после замены байта на {new_byte:02X} по смещению {offset}...")
                    run_dynamorio_analysis(brute_force_log_dir)

    except Exception as e:
        print(f"Ошибка при переборе: {str(e)}")

def main():
    while True:
        print("\nМеню DynamoRIO:")
        print("1 - Заменить байт в config_15")
        print("2 - Полный анализ (покрытие + регистры + стек)")
        print("3 - Перебор значений байта (00 и FF) для всех смещений с анализом")
        print("q - Выход")

        choice = input("> ").lower()
        
        if choice == '1':
            try:
                offset = int(input("Смещение(hex): "), 16)
                new_byte = int(input("Новый байт (hex): "), 16)
                replace_byte_in_file("config_15", offset, new_byte)
            except ValueError:
                print("Некорректный ввод!")
        
        elif choice == '2':
            manual_log_dir = f"manual_analysis_logs_{int(time.time())}"
            os.makedirs(manual_log_dir, exist_ok=True)
            run_dynamorio_analysis(manual_log_dir)
        
        elif choice == '3':
            brute_force_replace_byte("config_15")
            print("Перебор завершен.")
        
        elif choice == 'q':
            break

if __name__ == "__main__":
    main()