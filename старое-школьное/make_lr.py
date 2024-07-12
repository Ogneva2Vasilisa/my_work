def train_by_date(dataset):
    main_array = np.full([12,7,24],LinearRegression())
    data_array = [[[list() for _ in range(24)] for _ in range(7)] for _ in range(12)]
    pat = compile(r"(\d?\d)\.(\d?\d)\.(\d\d\d\d) (\d\d):00:00")

    for i,j in dataset:
        d,m,y,h = map(int, fullmatch(pat,i).groups())
        today = datetime.datetime(year=y,month=m,day=d,hour=h)
        data_array[today.month-1][today.weekday()][today.hour].append(j)

    for m in range(12):
        for d in range(7):
            for h in range(24):
                #вставь обучение сюда
    return  main_array
