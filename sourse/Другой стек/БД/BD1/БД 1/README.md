## Задача

Вся работа выполнялась в **postgres**.

1.    Определить структуру и семантику данных в исходном наборе.
2.    Определить факты.
3.    Определить измерения.
4.    Разработать схему данных «звезда» для анализа (БД1).
5.    Если набор данных позволяет, разработать схему данных «снежинка» для анализа (БД2).
6.    Заполнить БД1 и БД2 данными, использовав весь набор исходных данных.
7.    Разработать 5-8 аналитических запросов, на основании фактов, включая данные измерений.
8.    Оптимизировать запросы (используя индексы).
9.    Определить производительность запросов над каждой схемой. Построить графики выполнения.

---
## Решение

В качестве данных был выбран датасет Cybersecurity Threat Detection Logs, включающий в себя сгенерированные данные сетевого трафика и событий безопасности. Всего 6 млн строк событий. Столбцы в датасете_: timestamp, source_ip, dest_ip, protocol, action, threat_label, log_type, bytes_transferred, user_agent, request_path._ Выбранные колонки как факты: bytes_transferred (объем переданных данных в байтах). Выбранные измерения: остальные колонки.

Схема данных звезда

![](img/Pasted%20image%2020260107203353.png)

Схема данных снежинка

![](img/Pasted%20image%2020260107203436.png)


Далее было составлено несколько запросов:


| **Запрос**                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  | **Назначение**                                                                                                                                                            |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| ```sql<br>SELECT t.year, t.month, t.day, SUM(f.bytes_transferred) AS total_bytes<br><br>FROM fact_network_events f<br><br>JOIN dim_time t ON f.timestamp_id = t.timestamp_id<br><br>GROUP BY t.year, t.month, t.day ORDER BY t.year, t.month, t.day;<br>```<br><br>```sql<br>SELECT<br><br>    d.day, d.month, d.year,<br><br>    SUM(f.bytes_transferred) AS total_bytes<br><br>FROM fact_network_events f<br><br>JOIN dim_time t ON f.timestamp_id = t.timestamp_id<br><br>JOIN dim_date d ON t.date_id = d.date_id<br><br>GROUP BY d.year, d.month, d.day<br><br>ORDER BY d.year, d.month, d.day;<br>``` | Суммарный трафик по дням. Считает количество переданных байт для каждого дня. Факт — bytes_transferred, измерение — дата (dim_time).                                      |
| SELECT t.year, t.month, t.day, COUNT(*) AS event_count<br><br>FROM fact_network_events f<br><br>JOIN dim_time t ON f.timestamp_id = t.timestamp_id<br><br>GROUP BY t.year, t.month, t.day<br><br>ORDER BY t.year, t.month, t.day;<br><br>SELECT d.date AS day, COUNT(*) AS event_count<br><br>FROM fact_network_events f<br><br>JOIN dim_time t ON f.timestamp_id = t.timestamp_id<br><br>JOIN dim_date d ON t.date_id = d.date_id<br><br>GROUP BY d.date<br><br>ORDER BY d.date;                                                                                                                           | Считает количество всех событий за каждый день.                                                                                                                           |
| SELECT s.source_ip, SUM(f.bytes_transferred) AS total_bytes<br><br>FROM fact_network_events f<br><br>JOIN dim_source_ip s ON f.source_ip_id = s.source_ip_id<br><br>GROUP BY s.source_ip ORDER BY total_bytes DESC LIMIT 5;                                                                                                                                                                                                                                                                                                                                                                                 | Топ-5 IP источников по суммарному объему переданных данных.                                                                                                               |
| SELECT l.log_type, COUNT(*) AS blocked_count<br><br>FROM fact_network_events f<br><br>JOIN dim_log_type l ON f.log_type_id = l.log_type_id<br><br>JOIN dim_action a ON f.action_id = a.action_id<br><br>WHERE a.action = 'blocked'<br><br>GROUP BY l.log_type;                                                                                                                                                                                                                                                                                                                                              | Количество заблокированных соединений по типу лога.                                                                                                                       |
| SELECT<br><br>    l.log_type,<br><br>    a.action,<br><br>    COUNT(*) AS events_count<br><br>FROM fact_network_events f<br><br>JOIN dim_log_type l ON f.log_type_id = l.log_type_id<br><br>JOIN dim_action a ON f.action_id = a.action_id<br><br>GROUP BY l.log_type, a.action<br><br>ORDER BY l.log_type, a.action;                                                                                                                                                                                                                                                                                       | Количество событий по типу лога и действию (blocked/allowed).                                                                                                             |
| SELECT p.protocol, t.threat_label, SUM(f.bytes_transferred) AS total_bytes<br><br>FROM fact_network_events f<br><br>JOIN dim_protocol p ON f.protocol_id = p.protocol_id<br><br>JOIN dim_threat_label t ON f.threat_label_id = t.threat_label_id<br><br>GROUP BY p.protocol, t.threat_label;                                                                                                                                                                                                                                                                                                                | Считает суммарный трафик для каждого протокола и категории угроз. <br><br>Факт — bytes_transferred, измерения — протокол (dim_protocol) и метка угроз (dim_threat_label). |


---
## Анализ
