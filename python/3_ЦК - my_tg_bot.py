# 7017190530:AAFCKWP2Sud9hH6pQy1CK_wCgYNMXorbGfE
# 'register', 'login', 'predict','logout'
import os

os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import telebot
import pandas as pd
import csv
from PIL import Image
import numpy as np
import matplotlib.pyplot as plt
# ------------------------load_nn----------------
from keras.api.preprocessing import image
from keras._tf_keras.keras.models import model_from_json
import hashlib

print("Загружаю сеть из файлов")
json_file = open("mnist_model.json", "r")
loaded_model_json = json_file.read()
json_file.close()
model = model_from_json(loaded_model_json)
model.load_weights("mnist_model.weights.h5")
print("Загрузка сети завершена")
"""
test_dandelion_dir = os.path.join('data/data/test/squirrel')
test_grass_dir = os.path.join('data/data/test/man')
test_dandelion_names = os.listdir(test_dandelion_dir)
test_grass_names = os.listdir(test_grass_dir)
print('total testing squirrel images:', len(os.listdir(test_dandelion_dir)))
print('total testing man images:', len(os.listdir(test_grass_dir)))
test_datagen = ImageDataGenerator(rescale=1 / 1)
test_generator = test_datagen.flow_from_directory(
    'data/data/test/',
    classes=['squirrel', 'man'],
    target_size=(100, 100),
    batch_size=5,
    class_mode='binary',
    shuffle=False)
scores = model.evaluate(test_generator, verbose=0)
print("Точность работы загруженной сети на тестовых данных: %.2f%%" % (scores[1]*100))
"""

# ------------------------end_load_nn----------------

bot = telebot.TeleBot('7017190530:AAFCKWP2Sud9hH6pQy1CK_wCgYNMXorbGfE')


@bot.message_handler(commands=['start'])
def start(message):
    bot.send_message(message.chat.id, "Привет, вот список команд:")
    bot.send_message(message.chat.id,
                     "/register - Зарегистрироваться. Только зарегистрированные пользователи могут использовать бота.\n"
                     "/login - Войти в аккаунт.\n"
                     "/predict - Отправьте картинку, а бот скажет, это человек или белка.\n"
                     "/logout - Выйти из аккаунта.\n"
                     "/help - Список команд.")


@bot.message_handler(commands=['help'])
def help(message):
    bot.send_message(message.chat.id, "Вот список команд:")
    bot.send_message(message.chat.id,
                     "/register - Зарегистрироваться. Только зарегистрированные пользователи могут использовать бота.\n"
                     "/login - Войти в аккаунт.\n"
                     "/predict - Отправьте картинку, а бот скажет, это человек или белка.\n"
                     "/logout - Выйти из аккаунта.\n"
                     "/help - Список команд.")


@bot.message_handler(commands=['register'])
def register(message):
    mesg = bot.send_message(message.chat.id, 'Пожалуйста, введите пароль')
    bot.register_next_step_handler(mesg, test)


def test(message):
    if ',' in message.text or (';' in message.text or '/' in message.text):
        bot.send_message(message.chat.id, "Вы использовали , или ; или /\n"
                                          "Это нельзя использовать в пароле")
        return
    try:
        f_csv = pd.read_csv("ttt.csv", delimiter=",")
        print(f_csv)
        fields = []
        id_h = hashlib.sha256(str(message.from_user.id).encode('utf-8', errors='replace')).hexdigest()
        if (id_h not in list(f_csv["Id"])):
            print(message.from_user.username, id_h,
                  hashlib.md5(str(message.text).encode('utf-8', errors='replace')).hexdigest())
            fields = [message.from_user.username, id_h,
                      hashlib.md5(str(message.text).encode('utf-8', errors='replace')).hexdigest(), 0, 0]
            with open("ttt.csv", 'a+') as csvfile:
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(fields)
            bot.send_message(message.chat.id, "Вы успешно зарегистрированны!")
        else:
            bot.send_message(message.chat.id, "Вы уже зарегистрированны")

    except:
        bot.send_message(message.chat.id, "Что-то пошло не так, попробуйте позже.")


@bot.message_handler(commands=['login'])
def login(message):
    mesg = bot.send_message(message.chat.id, 'Пожалуйста, введите пароль')
    bot.register_next_step_handler(mesg, test1)


def test1(message):
    try:
        f_csv = pd.read_csv("ttt.csv", delimiter=",")
        # print(f_csv)
        id_h = hashlib.sha256(str(message.from_user.id).encode('utf-8', errors='replace')).hexdigest()
        if (id_h in list(f_csv["Id"])):
            for row in range(len(f_csv["Name"])):
                if (f_csv["Id"][row] == id_h and f_csv["password"][row] == hashlib.md5(
                        str(message.text).encode('utf-8', errors='replace')).hexdigest()):
                    f_csv.loc[row, "Flag"] = 1
                    f_csv.to_csv("ttt.csv", index=False)
                    bot.send_message(message.chat.id, "Вы успешно вошли в систему!")
                    return
        else:
            bot.send_message(message.chat.id, "Вы не зарегистрированны")
            return
        bot.send_message(message.chat.id, "Неверный пароль!")
        return
    except:
        bot.send_message(message.chat.id, "Что-то пошло не так, попробуйте позже.")


@bot.message_handler(commands=['logout'])
def logout(message):
    try:
        f_csv = pd.read_csv("ttt.csv", delimiter=",")
        # print(f_csv)
        id_h = hashlib.sha256(str(message.from_user.id).encode('utf-8', errors='replace')).hexdigest()
        if (id_h in list(f_csv["Id"])):
            for row in range(len(f_csv["Name"])):
                if (f_csv["Id"][row] == id_h and f_csv.loc[row, "Flag"] == 1):
                    f_csv.loc[row, "Flag"] = 0
                    f_csv.to_csv("ttt.csv", index=False)
                    bot.send_message(message.chat.id, "Вы успешно вышли из системы!")
                    return
                elif (f_csv["Id"][row] == id_h and f_csv.loc[row, "Flag"] == 0):
                    bot.send_message(message.chat.id, "Вы не входили в систему")
                    return
        else:
            bot.send_message(message.chat.id, "Вы не зарегистрированны")
    except:
        bot.send_message(message.chat.id, "Что-то пошло не так, попробуйте позже.")


@bot.message_handler(commands=['predict'])
def predict(message):
    try:
        f_csv = pd.read_csv("ttt.csv", delimiter=",")
        # print(f_csv)
        id_h = hashlib.sha256(str(message.from_user.id).encode('utf-8', errors='replace')).hexdigest()
        if (id_h in list(f_csv["Id"])):
            for row in range(len(f_csv["Name"])):
                if (f_csv["Id"][row] == id_h and f_csv.loc[row, "Flag"] == 1):
                    mesg = bot.send_message(message.chat.id, 'Пожалуйста, отправьте картинку')
                    bot.register_next_step_handler(mesg, test2)
                    return
                elif (f_csv["Id"][row] == id_h and f_csv.loc[row, "Flag"] == 0):
                    bot.send_message(message.chat.id, "Вы не вошли в систему")
                    return
        else:
            bot.send_message(message.chat.id, "Вы не зарегистрированны")
    except:
        bot.send_message(message.chat.id, "Что-то пошло не так, попробуйте позже.")


def test2(message):
    if message.photo:
        try:
            file_info = bot.get_file(message.photo[len(message.photo) - 1].file_id)
            downloaded_file = bot.download_file(file_info.file_path)
            src = 'C:/Users/Vasilisa/Music/NN3/tmp/' + message.photo[1].file_id
            with open(src, 'wb') as new_file:
                new_file.write(downloaded_file)
            img = Image.open(src)
            path = 'C:/Users/Vasilisa/Music/NN3/tmp/' + message.photo[1].file_id + '.jpg'
            img.save(path)
            bot.reply_to(message, "Фото добавлено")
            img = image.load_img(path, target_size=(100, 100))
            x = image.img_to_array(img)
            x = np.expand_dims(x, axis=0)
            classes = model.predict(x, batch_size=10)
            if classes[0] < 0.5:
                print(" is a squirrel")
                bot.send_message(message.chat.id, "Я думаю, это белка")
            else:
                print(" is a man")
                bot.send_message(message.chat.id, "Я думаю, это человек")
            os.remove(path)
            os.remove(src)
        except:
            bot.send_message(message.chat.id, "Что-то пошло не так, попробуйте позже.")


@bot.message_handler(content_types=['text', 'audio', 'document', 'photo', 'sticker', 'video', 'video_note',
                                    'voice', 'location', 'contact', 'new_chat_members', 'left_chat_member',
                                    'new_chat_title', 'new_chat_photo', 'delete_chat_photo', 'group_chat_created',
                                    'supergroup_chat_created', 'channel_chat_created', 'migrate_to_chat_id',
                                    'migrate_from_chat_id', 'pinned_message'])
def answer(message):
    bot.send_message(message.chat.id,
                     "Это прсто сообщение, я его не понимаю. Мне нужна команда. Введите /help, чтобы увидеть список команд")
    # if message.text == "Кодовое слово":
    print(message.text, message.from_user.username, message.from_user.id)


bot.polling(none_stop=True, interval=0)
