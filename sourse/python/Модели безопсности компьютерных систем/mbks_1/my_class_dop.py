import os
import subprocess
import time
import json
import shutil
from collections import defaultdict

ERROR_LOG = "error_log.json"
MAX_CONSECUTIVE_ERRORS = 1
BACKUP_FILE = "uuu.xlsx.original"
TARGET_FILE = "uuu.xlsx"
EXCEL_PATH = r"C:\Program Files\Microsoft Office\root\Office16\EXCEL.EXE"  # Путь к Excel

# Конфигурация
ERROR_LOG = "error_log.json"
drrun_path = r"C:\Users\Vasilisa\Downloads\DynamoRIO-Windows-11.90.20168\bin64\drrun.exe"

def run_dynamorio_analysis_0(log_dir):
    """Запуск анализа с DynamoRIO и сохранение результатов в указанную папку"""
    try:
        if not os.path.exists(EXCEL_PATH):
            print("Ошибка: exel.exe не найден!")
            return False

        os.makedirs(log_dir, exist_ok=True)

        # 1. Собираем покрытие кода
        coverage_cmd = [
            drrun_path,
            "-t", "drcov",
            "-dump_text",
            "-logdir", log_dir,
            "--", EXCEL_PATH
        ]

        print("[+] Запуск DynamoRIO (анализ покрытия)...")
        subprocess.run(coverage_cmd, check=True)

        # Обработка результатов
        print("\n[+] Результаты:")
        for fname in os.listdir(log_dir):
            if fname.startswith("drcov.exel.exe"):
                log_file = os.path.join(log_dir, fname)
                print(f"[+] Файл покрытия: {log_file}")
                return True
        
        print("[-] Файл покрытия не создан")
        return False

    except subprocess.CalledProcessError as e:
        print(f"Ошибка DynamoRIO: {e}")
        return False

def setup_dynamorio():
    """Проверка наличия DynamoRIO и клиентских инструментов"""
    dynamorio_home = r"C:\Users\Vasilisa\Downloads\DynamoRIO-Windows-11.90.20168"
    dr = dynamorio_home + "\\\\" + "bin64" + "\\\\" + "drrun.exe"
    
    return dr 

def log_error(error_data):
    """Логирование ошибок в JSON файл"""
    try:
        # Проверяем, существует ли файл и не пустой ли он
        file_exists = os.path.exists(ERROR_LOG) and os.path.getsize(ERROR_LOG) > 0
        
        with open(ERROR_LOG, 'a', encoding='utf-8') as f:
            if not file_exists:
                f.write('[\n')  # Начинаем новый JSON массив, если файл пустой
            else:
                f.write(',\n')  # Добавляем запятую перед новым элементом

            json.dump(error_data, f, ensure_ascii=False)  # Записываем данные в JSON

    except Exception as e:
        print(f"Ошибка логирования: {str(e)}")

def run_dynamorio_analysis(log_dir, modifications):
    """Запуск анализа с DynamoRIO (адаптированная версия из вашего кода)"""
    try:
        drrun_path = setup_dynamorio()
        if not drrun_path:
            return False

        cmd = [
            drrun_path,
            "-t", "drcov",
            "-dump_text",
            "-logdir", log_dir,
            "--", EXCEL_PATH
        ]

        result = subprocess.run(
            cmd,
            capture_output=True,
            text=True,
            timeout=10
        )

        error_data = {
            "timestamp": time.time(),
            "modifications": modifications,
            "returncode": result.returncode,
            "log_dir": log_dir,
            "stdout": result.stdout,
            "stderr": result.stderr
        }

        if result.returncode != 0:
            error_data["error_type"] = f"Crash with code {result.returncode}"
            log_error(error_data)
            return False

        return True

    except Exception as e:
        error_data = {
            "timestamp": time.time(),
            "error": str(e),
            "modifications": modifications
        }
        log_error(error_data)
        return False

class CoverageGuidedFuzzer:
    def __init__(self, max_iterations=100):
        self.coverage_data = {}
        self.interesting_cases = []
        self.error_log = "coverage_errors.json"
        self.backup_file = f"{TARGET_FILE}.original"
        self.max_iterations = max_iterations
        self.current_iteration = 0
        self.max_offset = 0x38
        self.total_possible_addresses = 0
        self.coverage_history = []
        self.failed_mutations = 0  # Счетчик неудачных мутаций
        self.success_counter = 0  # Счетчик успешных мутаций
        
    def calculate_total_addresses(self, log_dir):
        """Подсчет количества базовых блоков в модуле vuln15.exe (ID 0)"""
        total_blocks = 0
        for fname in os.listdir(log_dir):
            if fname.startswith("drcov"):
                with open(os.path.join(log_dir, fname), 'r') as f:
                    for line in f:
                        if line.startswith("BB Table:"):
                            parts = line.split()
                            total_blocks = int(parts[2].strip(' '))
                            break
        self.total_possible_addresses = total_blocks
        print(f"▓ Найдено {self.total_possible_addresses} базовых блоков в vuln15.exe")
        return self.total_possible_addresses

    def analyze_coverage(self, log_dir):
        """Анализ логов покрытия DynamoRIO для vuln15.exe"""
        coverage = set()
        for fname in os.listdir(log_dir):
            if fname.startswith("drcov"):
                with open(os.path.join(log_dir, fname), 'r') as f:
                    for line in f:
                        if line.startswith("module["):
                            parts = line.split()
                            module_id = int(parts[1].strip('[]:'))
                            if module_id == 0 or module_id == 7:  # Только модуль vuln15.exe
                                addr = parts[2]
                                coverage.add(addr)
        return coverage
    
    def save_interesting_case(self, modification, coverage):
        """Сохраняем интересные кейсы, которые увеличили покрытие"""
        self.interesting_cases.append({
            "modification": modification,
            "coverage": coverage,
            "timestamp": time.time()
        })

    def is_new_coverage(self, new_coverage):
        """Проверяем, дает ли новый кейс увеличение покрытия"""
        if not self.coverage_data:
            return True
            
        for addr in new_coverage:
            if addr not in self.coverage_data:
                return True
        return False
    
    def update_coverage(self, new_coverage):
        """Обновляем данные о покрытии"""
        for addr in new_coverage:
            self.coverage_data[addr] = self.coverage_data.get(addr, 0) + 1
        
        # Сохраняем текущее состояние покрытия
        self.coverage_history.append({
            "iteration": self.current_iteration,
            "covered_addresses": len(self.coverage_data),
            "timestamp": time.time()
        })
    
    def mutate_byte(self, filename, offset, value):
        """Мутация одного байта"""
        with open(filename, 'r+b') as f:
            f.seek(offset)
            f.write(bytes([value]))
    
    def analyze_blocks(self, log_dir):
        """Анализ количества базовых блоков в логах DynamoRIO"""
        for fname in os.listdir(log_dir):
            if fname.startswith("drcov"):
                with open(os.path.join(log_dir, fname), 'r') as f:
                    for line in f:
                        if line.startswith("BB Table:"):
                            parts = line.split()
                            return int(parts[2].strip(' '))
        return 0

    def smart_fuzz(self):
        """Интеллектуальный фаззинг на основе покрытия"""
        # Создаем резервную копию
        if not os.path.exists(self.backup_file):
            shutil.copyfile(TARGET_FILE, self.backup_file)
        
        try:
            # Получаем начальное количество блоков
            initial_log_dir = "initial_analysis"
            os.makedirs(initial_log_dir, exist_ok=True)
            initial_blocks=0
            current_best_blocks=0
            if run_dynamorio_analysis(initial_log_dir, {}):
                initial_blocks = self.analyze_blocks(initial_log_dir)
                current_best_blocks = initial_blocks
                print(f"▓ Начальное количество блоков: {initial_blocks}")
            else:
                print("▓ Ошибка при инициализации анализа")
                return

            # Получаем только первые max_offset байт для мутации
            target_offsets = list(range(0, self.max_offset))
            current_offset_index = 0  # Индекс текущего смещения для мутации

            while self.current_iteration < self.max_iterations:
                self.current_iteration += 1
                print(f"\n▓ Итерация {self.current_iteration}/{self.max_iterations}")
                print(f"▓ Текущее лучшее количество блоков: {current_best_blocks}")
                print(f"▓ Неудачных мутаций подряд: {self.failed_mutations}/10")
                print(f"▓ Успешных мутаций: {self.success_counter}")

                # Восстанавливаем оригинальный файл только если нет успешных мутаций
                if not self.interesting_cases:
                    shutil.copyfile(self.backup_file, TARGET_FILE)

                # Проверяем, есть ли еще смещения для мутации
                if current_offset_index < len(target_offsets):
                    offset = target_offsets[current_offset_index]
                    value = 0xFF if (self.current_iteration % 2) else 0x00
                    self.mutate_byte(TARGET_FILE, offset, value)
                    modification = {offset: value}
                    print(f"▓ Мутация: смещение 0x{offset:02X} -> 0x{value:02X}")

                    # Запускаем анализ
                    log_dir = f"fuzz_logs/iter_{self.current_iteration}"
                    os.makedirs(log_dir, exist_ok=True)

                    if run_dynamorio_analysis(log_dir, modification):
                        new_blocks = self.analyze_blocks(log_dir)
                        
                        if new_blocks > current_best_blocks:
                            print(f"▓ Найдено увеличение количества блоков! {current_best_blocks} -> {new_blocks}")
                            current_best_blocks = new_blocks
                            self.success_counter += 1
                            shutil.copyfile(TARGET_FILE, f"config_15.success_{self.success_counter}")
                            self.failed_mutations = 0  # Сбрасываем счетчик неудачных мутаций
                        else:
                            self.failed_mutations += 1
                            print(f"▓ Мутация не увеличила покрытие ({self.failed_mutations}/10)")
                            # Если мутация не удалась, переходим к следующему смещению
                            current_offset_index += 1

                        if self.failed_mutations >= 10:
                            try:
                                shutil.copyfile(f"{TARGET_FILE}.success_{self.success_counter-1}", TARGET_FILE)
                            except:
                                shutil.copyfile(self.backup_file, TARGET_FILE)
                            self.failed_mutations = 0
                            print("▓ Откат к последней успешной версии")
                    else:
                        # Если анализ не удался, переходим к следующему смещению
                        current_offset_index += 1
                        print(f"▓ Анализ не удался, приложение могло упасть")
                        self.failed_mutations = 0  # Сбрасываем счетчик неудачных мутаций


                # Периодически сохраняем прогресс
                if self.current_iteration % 10 == 0:
                    self.save_progress()

        finally:
            # Восстанавливаем оригинальный файл
            shutil.copyfile(self.backup_file, TARGET_FILE)
            self.save_progress()
            print(f"\n▓ Фаззинг завершен.")
            print(f"▓ Первоначальное покрытие {initial_blocks} блоков")
            print(f"▓ Итоговое покрытие {current_best_blocks} блоков")
            print(f"▓ Всего успешных мутаций: {self.success_counter}")
    
    def mutate_based_on_coverage(self, base_modification):
        """Мутация на основе предыдущих успешных кейсов"""
        new_mod = {}
        for offset, value in base_modification.items():
            if offset >= self.max_offset:  # Пропускаем смещения за пределами max_offset
                continue
            # Пробуем разные варианты мутаций
            if self.current_iteration % 3 == 0:
                new_mod[offset] = value ^ 0xFF  # Инвертируем байт
            elif self.current_iteration % 3 == 1:
                new_mod[offset] = (value + 1) % 256  # Инкремент
            else:
                new_mod[offset] = (value - 1) % 256  # Декремент
        return new_mod
    
    def save_progress(self):
        """Сохранение прогресса фаззинга"""
        progress = {
            "interesting_cases": self.interesting_cases,
            "iteration": self.current_iteration,
            "total_possible_addresses": self.total_possible_addresses,
            "coverage_history": self.coverage_history,
            "statistics": {
                "total_iterations": self.current_iteration,
                "total_covered_addresses": len(self.coverage_data),
                "total_interesting_cases": len(self.interesting_cases),
                "last_update": time.ctime()
            }
        }
        with open("fuzzer_progress.json", 'w') as f:
            json.dump(progress, f, indent=4) 