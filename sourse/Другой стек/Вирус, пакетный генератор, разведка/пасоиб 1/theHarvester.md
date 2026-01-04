https://codeby.net/threads/theharvester.85914/

Здесь нужны API ключи :(

zoom eye
пароль
120928Vas!@#$

### Команды

```
theHarvester -d 49.50.236.20 -b all -f report.html

```

---
### Настройка ключей API для максимальной мощности​[](https://codeby.net/threads/theharvester.85914/#-nastroika-klyuchei-api-dlya-maksimal-noi-moshchnosti "Постоянная ссылка")

Без настроенных ключей theHarvester использует только 30% своих возможностей. С правильно настроенными API вы получите в 10 раз больше данных.  

#### Создание конфигурационного файла​[](https://codeby.net/threads/theharvester.85914/#-sozdaniye-konfiguratsionnogo-faila "Постоянная ссылка")

Bash:

```bash
# Создание директории и файла конфигурации
mkdir -p ~/.theHarvester
nano ~/.theHarvester/api-keys.yaml
```

#### Полная конфигурация всех доступных API (2025)​[](https://codeby.net/threads/theharvester.85914/#-polnaya-konfiguratsiya-vsekh-dostupnykh-api-2025 "Постоянная ссылка")

YAML:

```yaml
apikeys:
  # Бесплатные ключи (регистрация за 2 минуты)
  github:
    key: "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"  # https://github.com/settings/tokens
    # Бесплатно: 5000 запросов/час
  hunter:
    key: "782e433a9cece090c386a783d9da2b4eff9ea57f"  # https://hunter.io/api
    # Бесплатно: 50 поисков/месяц
  intelx:
    key: "18c9cf29-6ca2-48d0-9b82-4d7fde487578"  # https://intelx.io/
    # Бесплатно: 1000 поисков/месяц

  shodan:
    key: "sTXpu636o6EmoXfSnNtZqpBMVkMCzI9H"  # https://shodan.io/
    # Бесплатно: 100 результатов/месяц
    # 💡 Академическая лицензия: бесплатно для студентов

  virustotal:
    key: "cc456c805372365776f6ef15883378d339a8030f6d1f594a03c90cdc5a9c4e41"  # https://virustotal.com/
    # Бесплатно: 500 запросов/день

  zoomeye:
    username: "vasilisa.chernikova@internet.ru"  # https://zoomeye.org/
    password: "F3616cC5-E721-af0dc-e240-17453a457fd"
    # Бесплатно: 10000 результатов/месяц
  # Платные API (рекомендуемые для профессионалов)
```

#### Пошаговое получение ключей (на примере Shodan)​[](https://codeby.net/threads/theharvester.85914/#-poshagovoye-polucheniye-klyuchei-na-primere-shodan "Постоянная ссылка")

1. **Регистрация:** Перейдите на shodan.io и создайте аккаунт
2. **Подтверждение почты:** Активируйте учётную запись через письмо
3. **Получение ключа:** Зайдите в Account → API Key
4. **Копирование:** Скопируйте 32-символьный ключ
5. **Добавление в конфигурацию:** Вставьте в api-keys.yaml
6. **Проверка:** `theHarvester -d test.com -b shodan`
---
### хело

"aahmad@unimas.my","gkartini@unimas.my","hrosita@unimas.my","jvictoria@unimas.my","kkamarudin@unimas.my","laudrey@unimas.my","lterry@unimas.my","ssalawati@unimas.my","sshanti@unimas.my","wrafeah@unimas.my"


![](Pasted%20image%2020251120003946.png)