import os

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'
import numpy as np
import pickle
import tensorflow as tf
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
from PIL import Image
from scipy import interpolate
from itertools import cycle
from sklearn import svm, datasets
from sklearn.metrics import roc_auc_score
from sklearn.metrics import roc_curve, auc
from sklearn.preprocessing import label_binarize
from keras.api.preprocessing import image
from sklearn.multiclass import OneVsRestClassifier
from sklearn.model_selection import train_test_split
from keras.src.legacy.preprocessing.image import ImageDataGenerator

train_dandelion_dir = os.path.join('data/data/train/squirrel')
train_grass_dir = os.path.join('data/data/train/man')
valid_dandelion_dir = os.path.join('data/data/val/squirrel')
valid_grass_dir = os.path.join('data/data/val/man')
train_dandelion_names = os.listdir(train_dandelion_dir)
train_grass_names = os.listdir(train_grass_dir)
validation_grass_names = os.listdir(valid_grass_dir)

print('total training squirrel images:', len(os.listdir(train_dandelion_dir)))
print('total training man images:', len(os.listdir(train_grass_dir)))
print('total validation squirrel images:', len(os.listdir(valid_dandelion_dir)))
print('total validation man images:', len(os.listdir(valid_grass_dir)))

nrows = 4
ncols = 4

pic_index = 0

fig = plt.gcf()
fig.set_size_inches(ncols * 4, nrows * 4)

pic_index += 8
next_dandelion_pic = [os.path.join(train_dandelion_dir, fname)
                      for fname in train_dandelion_names[pic_index - 8:pic_index]]
next_grass_pic = [os.path.join(train_grass_dir, fname)
                  for fname in train_grass_names[pic_index - 8:pic_index]]

for i, img_path in enumerate(next_dandelion_pic + next_grass_pic):
    sp = plt.subplot(nrows, ncols, i + 1)
    sp.axis('Off')

    img = mpimg.imread(img_path)
    plt.imshow(img)

# plt.show()

train_datagen = ImageDataGenerator(rescale=1 / 1)
validation_datagen = ImageDataGenerator(rescale=1 / 1)

train_generator = train_datagen.flow_from_directory(
    'data/data/train/',
    classes=['squirrel', 'man'],
    target_size=(100, 100),
    batch_size=4,
    class_mode='binary')

validation_generator = validation_datagen.flow_from_directory(
    'data/data/val/',
    classes=['squirrel', 'man'],
    target_size=(100, 100),
    batch_size=4,
    class_mode='binary',
    shuffle=False)
'''
model = tf.keras.Sequential([
    #tf.keras.layers.Flatten(input_shape=(100, 100, 3)),
    tf.keras.layers.Conv2D(32, (3, 3), padding='same', activation='relu', input_shape=(100, 100, 3)),
    tf.keras.layers.MaxPooling2D((2, 2), strides=2),
    tf.keras.layers.Dense(128, activation='relu'),
    tf.keras.layers.Conv2D(64, (3, 3), padding='same', activation='relu'),
    tf.keras.layers.MaxPooling2D((2, 2), strides=2),
    tf.keras.layers.Flatten(),
    tf.keras.layers.Dense(1, activation='relu'),
])

'''
model = tf.keras.models.Sequential([tf.keras.layers.Flatten(input_shape=(100, 100, 3)),
                                    tf.keras.layers.Dense(128, activation=tf.nn.relu),
                                    tf.keras.layers.Dense(1, activation=tf.nn.sigmoid)])


model.summary()
# tf.keras.optimizers.Adam()
model.compile(optimizer="adam",
              loss='binary_crossentropy',
              metrics=['accuracy'])

history = model.fit(train_generator,
                    steps_per_epoch=8,
                    batch_size=24,
                    epochs=30,
                    verbose=1,
                    validation_data=validation_generator,
                    validation_steps=8)
test_datagen = ImageDataGenerator(rescale=1 / 1)
test_generator = test_datagen.flow_from_directory(
    'data/data/test/',
    classes=['squirrel', 'man'],
    target_size=(100, 100),
    batch_size=5,
    class_mode='binary',
    shuffle=False)
scores = model.evaluate(test_generator, verbose=0)
print("Точность работы загруженной сети на тестовых данных: %.2f%%" % (scores[1] * 100))

print("Сохраняем сеть")
# Сохраняем сеть для последующего использования
# Генерируем описание модели в формате json
model_json = model.to_json()
json_file = open("mnist_model.json", "w")
# Записываем архитектуру сети в файл
json_file.write(model_json)
json_file.close()
# Записываем данные о весах в файл
model.save_weights("mnist_model.weights.h5")
print("Сохранение сети завершено")

uploaded = ['test.jpg']

for fn in uploaded:
    path = fn
    img = image.load_img(path, target_size=(100, 100))
    x = image.img_to_array(img)
    plt.imshow(x / 255.)
    x = np.expand_dims(x, axis=0)
    images = np.vstack([x])
    classes = model.predict(images, batch_size=10)
    print(classes[0])
    if classes[0] < 0.5:
        print(fn + " is a squirrel")
    else:
        print(fn + " is a man")
