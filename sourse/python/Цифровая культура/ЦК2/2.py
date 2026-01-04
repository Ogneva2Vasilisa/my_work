import argparse
import requests
import pprint
import matplotlib.pyplot as plt
from bs4 import BeautifulSoup

def stud(date_r, group):
    # date_r = date = "2024-4-5"  # - значит, что в этой неделе будет этот день
    # group = str(38653)  # - уникальный id группы
    response = requests.get('https://ruz.spbstu.ru/api/v1/ruz/scheduler/' + group + '?date=' + date_r)
    #print(response.json())
    text = response.json()["days"]
    rasp = []
    week_is_odd = response.json()["week"]["is_odd"]  # True - нечетная
    for i in range(len(text)):
        rasp.append([])
        rasp[i].append({"date": text[i]["date"]})
        for j in range(len(text[i]["lessons"])):
            rasp[i].append(dict())
            rasp[i][j]["subject"] = text[i]["lessons"][j]["subject"]
            # print(text[i]["lessons"][j])
            rasp[i][j]["time_start"] = text[i]["lessons"][j]["time_start"]
            rasp[i][j]["time_end"] = text[i]["lessons"][j]["time_end"]
            rasp[i][j]["type"] = text[i]["lessons"][j]["typeObj"]["name"]
            rasp[i][j]["auditories"] = text[i]["lessons"][j]["auditories"][0]["name"]
            try:
                rasp[i][j]["teachers"] = text[i]["lessons"][j]["teachers"][0]["full_name"]
            except:
                rasp[i][j]["teachers"] = "None"
    if week_is_odd:
        print("Неделя нечетная")
    else:
        print("Неделя четная")
    for i in range(len(rasp)):
        print("День " + str(i))
        pprint.pprint(rasp[i])

    graph = []
    date = []
    for i in range(len(text)):
        graph.append(len(text[i]["lessons"]))  # для графика
        date.append(text[i]["date"])  # для графика

    index = [0, 1, 2, 3, 4]
    values = [5, 7, 3, 4, 6]
    plt.figure(figsize=(10,7))
    plt.bar(date, graph)
    plt.title(f'ID - Группа {group}')
    plt.xlabel('День')
    plt.ylabel('Количество занятий')
    plt.show()

def aud(date_r, b, a):
    # date_r = date = "2024-4-5"  # - значит, что в этой неделе будет этот день
    # b = str(1)  # - уникальный id здания
    # a = str(546)  # - уникальный id аудитории
    # /api/v1/ruz/buildings/1/rooms/546/scheduler?date=2024-5-27
    response = requests.get('https://ruz.spbstu.ru/api/v1/ruz/buildings/' + b + '/rooms/' + a + '/scheduler?date=' + date_r)
    print(response.json())
    text = response.json()["days"]
    rasp = []
    week_is_odd = response.json()["week"]["is_odd"]  # True - нечетная
    for i in range(len(text)):
        rasp.append([])
        rasp[i].append({"date": text[i]["date"]})
        for j in range(len(text[i]["lessons"])):
            rasp[i].append(dict())
            rasp[i][j]["subject"] = text[i]["lessons"][j]["subject"]
            # print(text[i]["lessons"][j])
            rasp[i][j]["time_start"] = text[i]["lessons"][j]["time_start"]
            rasp[i][j]["time_end"] = text[i]["lessons"][j]["time_end"]
            rasp[i][j]["type"] = text[i]["lessons"][j]["typeObj"]["name"]
            rasp[i][j]["auditories"] = text[i]["lessons"][j]["auditories"][0]["name"]
            try:
                rasp[i][j]["teachers"] = text[i]["lessons"][j]["teachers"][0]["full_name"]
            except:
                rasp[i][j]["teachers"] = "None"
    if week_is_odd:
        print("Неделя нечетная")
    else:
        print("Неделя четная")
    for i in range(len(rasp)):
        print("День " + str(i))
        pprint.pprint(rasp[i])

    graph = []
    date = []
    for i in range(len(text)):
        graph.append(len(text[i]["lessons"]))  # для графика
        date.append(text[i]["date"])  # для графика

    index = [0, 1, 2, 3, 4]
    values = [5, 7, 3, 4, 6]
    plt.figure(figsize=(10, 7))
    plt.bar(date, graph)
    plt.title(f'ID - Здание {b}, аудитория {a}')
    plt.xlabel('День')
    plt.ylabel('Количество занятий')
    plt.show()

def gz_to_id(b, a):
    resp=[]
    # date_r = date = "2024-4-5"  # - значит, что в этой неделе будет этот день
    # b = str(1)  # - уникальный id здания
    # a = str(546)  # - уникальный id аудитории
    # /api/v1/ruz/buildings/1/rooms/546/scheduler?date=2024-5-27
    response = requests.get('https://ruz.spbstu.ru/api/v1/ruz/buildings/')
    text=response.json()["buildings"]
    #print(text)
    for i in range(len(text)):
        if text[i]['name']==b:
            resp.append(str(text[i]['id']))
            response = requests.get('https://ruz.spbstu.ru/api/v1/ruz/buildings/'+str(resp[0])+"/rooms")
            text = response.json()["rooms"]
            for j in range(len(text)):
                if text[j]['name'] == a:
                    resp.append(str(text[j]['id']))
                    return resp
            return None
    return None


def g_to_id(id):
    req_get_group_id = requests.get('https://ruz.spbstu.ru/search/groups?q='+id)
    soup = BeautifulSoup(req_get_group_id.text, 'html.parser')
    groups = []
    for link in soup.find_all('a'):
        if link.get('class'):
            if link.get('class').count("groups-list__link"):
                groups.append(
                    [link.get_text(), link.get('href')[link.get('href').rfind('/') + 1:]])
    if len(groups) > 1:
        print("Введите правильный номер группы")
        return None
    elif len(groups) < 1:
        print("Не найдено ни одной группы.")
        return None
    else:
        return groups[0][1]

def main():
    p = argparse.ArgumentParser()
    # file system args
    p.add_argument("-group", "--g", nargs=2, help="Rasp for your group. Args: data, id")
    p.add_argument("-auditor", "--a", nargs=3, help="Rasp in auditore. Args: data, building, id")
    p.add_argument("-select", "--s", nargs=1, help="Список всех зданий")
    args = p.parse_args()  # массив с аргументами
    print(args)
    if args.g:
        if g_to_id(args.g[1]):
            stud(args.g[0], g_to_id(args.g[1]))
    if args.a:
        if gz_to_id(args.a[1],args.a[2]):
            aud(args.a[0], gz_to_id(args.a[1],args.a[2])[0], gz_to_id(args.a[1],args.a[2])[1])
    if args.s:
        print("1-й учебный корпус\n"
                                        "11-й учебный корпус\n"
                                        "15-й учебный корпус\n"
                                        "16-й учебный корпус\n"
                                        "17-й учебный корпус\n"
                                        "2-й учебный корпус\n"
                                        "3-й учебный корпус\n"
                                        "4-й учебный корпус\n"
                                        "48 учебный корпус\n"
                                        "5-й учебный корпус\n"
                                        "50 учебный корпус\n"
                                        "6-й учебный корпус\n"
                                        "9-й учебный корпус\n"
                                        "DL\n"
                                        "ВИТ\n"
                                        "Военная кафедра\n"
                                        "Гидробашня\n"
                                        "Гидротехнический корпус-1 Северная пристройка\n"
                                        "Гидротехнический корпус-1\n"
                                        "Гидротехнический корпус-2\n"
                                        "Главное здание\n"
                                        "Завод «Силовые Машины»\n"
                                        "Завод АО «Невский завод»\n"
                                        "ИМОП\n"
                                        "Институт ядерной энергетики (филиал ФГАОУ ВО СПбПУ) г. Сосновый Бор\n"
                                        "Лабораторный корпус\n"
                                        "Литейный корпус\n"
                                        "Механический корпус\n"
                                        "НОЦ СЛК\n"
                                        "Научно-исследовательский корпус\n"
                                        "Не определено\n"
                                        "СЗГМУ им. И.И.Мечникова\n"
                                        "Санкт-Петербургский институт машиностроения\n"
                                        "Спорткомплекс\n"
                                        "ТВН\n"
                                        "ТВН-2\n"
                                        "Химический корпус\n"
                                        "Холдинг «ЛЕНПОЛИГРАФМАШ»\n"
                                        "ЦНИИ РТК")

if __name__ == '__main__':
    main()
