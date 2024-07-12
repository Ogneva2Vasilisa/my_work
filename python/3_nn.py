import os
os.environ['TF_CPP_MIN_LOG_LEVEL'] = '2'
import numpy as np
from keras.api.preprocessing import image
from keras._tf_keras.keras.models import model_from_json

from sklearn.multiclass import OneVsRestClassifier
from sklearn.model_selection import train_test_split
from keras.src.legacy.preprocessing.image import ImageDataGenerator
print("Загружаю сеть из файлов")
# Загружаем данные об архитектуре сети
json_file = open("mnist_model.json", "r")
loaded_model_json = json_file.read()
json_file.close()
# Создаем модель
model = model_from_json(loaded_model_json)
# Загружаем сохраненные веса в модель
model.load_weights("mnist_model.weights.h5")
print("Загрузка сети завершена")

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

uploaded = ['test_m.png']

for fn in uploaded:
    path = fn
    img = image.load_img(path, target_size=(100, 100))
    x = image.img_to_array(img)
    x = np.expand_dims(x, axis=0)
    print(x.shape)
    classes = model.predict(x, batch_size=10)
    print(classes)
    print(classes[0])
    if classes[0] < 0.5:
        print(fn + " is a squirrel")
    else:
        print(fn + " is a man")
