## **Задача**
Создать нейронную сеть, отличающая человека или белочку на входном изображении. Подключить эту модель к телеграмм-боту и проверить работоспособность.
### 1. Модель
Использованные библиотеки:
```Python
import numpy as np
import pickle
import tensorflow as tf
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from PIL import Image
from scipy import interpolate
from itertools import cycle
from sklearn import *
from keras import *
```
Создание модели
![](../../img/Pasted%20image%2020250617210315.png)
### 2. Бот
Данные хранились в открытом виде в .сsv файле, поэтому было решено добавить одностороннее шифрование Id пользователя из телеграмма и пароля.
Была подключена библиотека hashlib, и использованы алгоритмы SHA-256 и MD5.
### 3. Подключение ngrok
Ngrok выступает сервисом, который предоставляет сервер в интернете и проводит туннель на локальный хост
**ngrok http** [**http://127.0.0.1:5000**](http://127.0.0.1:5000)
![](file:///C:/Users/Vasilisa/AppData/Local/Packages/oice_16_974fa576_32c1d314_b7f/AC/Temp/msohtmlclip1/01/clip_image002.jpg)
### 4. Зарегистрировать созданный webhook в Telegram.
В браузере был отправлен запрос
[https://api.telegram.org/bot{token_bot}/setWebhook?url={URL}](https://api.telegram.org/bot%7btoken_bot%7d/setWebhook?url=%7bURL%7d),
где token – это токен бота
URL – веб-приложение с которым надо связать бота
# Работа бота:
![](../../img/Pasted%20image%2020250617211837.png)
![](../../img/Pasted%20image%2020250617211901.png)
![](../../img/Pasted%20image%2020250617211929.png)