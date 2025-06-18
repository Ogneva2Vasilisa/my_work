import pybind11
from distutils.core import setup, Extension

ext_modules = [
    Extension(
        'MyString', # имя библиотеки собранной pybind11
        ['E:\осень 2024!!\chernikova_v_m_lab_1_mystring-master\MyString_13-09-2024\MyString_13-09-2024.cpp'], # Тестовый файлик который компилируем
        include_dirs=[pybind11.get_include()],  # не забываем добавить инклюды pybind11
        language='c++', # Указываем язык
        extra_compile_args=['-std=c++11'], # флаг с++11
    ),
]

setup(
    name='MyString', # имя библиотеки собранной pybind11
    version='1.0.0',
    author='iukjkl',
    author_email='iuhlkj.ru',
    description='pybind11 extension',
    ext_modules=ext_modules,
    requires=['pybind11'],  # Указываем зависимость от pybind11
    package_dir = {'': 'lib'}
)