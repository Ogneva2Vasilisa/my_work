def train_by_date(dataset):
    from re import compile, fullmatch
    main_array = np.full([4,7,24],LinearRegression())
    data_array = [[[list() for _ in range(24)] for _ in range(7)] for _ in range(4)]
    pat = compile(r"(\d?\d)\.(\d?\d)\.(\d\d\d\d) (\d\d):00:00")

    for i,j in dataset:
        d,m,y,h = map(int, fullmatch(pat,i).groups())
        today = datetime.datetime(year=y,month=m,day=d,hour=h)
        data_array[(today.month-1)//3][today.weekday()][today.hour].append(((d,m,y,h),j))

    for s in range(4):
        for d in range(7):
            for h in range(24):
                dat=data_array[s][d][h]
                main_array[s,d,h].fit([i[0] for i in dat], [i[1] for i in dat])

    return main_array
