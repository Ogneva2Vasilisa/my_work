import argparse
import os
import shutil
from winreg import *


def my_choise(string):
    ss = list(string.split("\\"))[0]
    # print(ss)
    if ss == "HKEY_CLASSES_ROOT":
        return HKEY_CLASSES_ROOT
    if ss == "HKEY_LOCAL_MACHINE":
        return HKEY_LOCAL_MACHINE
    if ss == "HKEY_USERS":
        return HKEY_USERS
    if ss == "HKEY_DYN_DATA":
        return HKEY_DYN_DATA
    if ss == "HKEY_CURRENT_CONFIG":
        return HKEY_CURRENT_CONFIG
    if ss == "HKEY_CURRENT_USER":
        return HKEY_CURRENT_USER
    if ss == "HKEY_PERFORMANCE_DATA":
        return HKEY_PERFORMANCE_DATA


def search_registry(path_root, key_val, meaning):
    #print("in dir: ", path_root, key_val, meaning)
    try:
        key = OpenKey(path_root, key_val)
        num_keys = QueryInfoKey(key)[0]
        #print("info =", QueryInfoKey(key), end=" ")
        for i in range(QueryInfoKey(key)[1]):
            try:
                sub_value = EnumValue(key, i)
                #print("sub - ", sub_value)
                if meaning == str(sub_value[1]):
                    print('!', key_val, "\\", str(sub_value[0]))
            except OSError:
                pass
        for i in range(num_keys):
            sub_key_name = EnumKey(key, i)
            sub_key = OpenKey(key, sub_key_name)
            search_registry(path_root, key_val + "\\" + sub_key_name, meaning)
            CloseKey(sub_key)
        CloseKey(key)
        CloseKey(path_root)
    except PermissionError:
        print("Permission denied")


def main():
    p = argparse.ArgumentParser()
    # file system args
    p.add_argument("-create_file", "--create", help="Creating of file")
    p.add_argument("-delete_file", "--delete", help="Delete file")
    p.add_argument("-find_string", "--find", help="Find file")
    p.add_argument("-write_to_file", "--write", nargs=2, help="To write anything into file")
    p.add_argument("-read_from_file", "--read", help="To read information from file")
    p.add_argument("-copy_file_to_dir", "--copy", nargs=2, help="To copy file between directories")
    p.add_argument("-rename_file", "--rename", nargs=2, help="To rename the file")
    p.add_argument("-create_key", "--cr_key", nargs=2, help="To create the key")
    p.add_argument("-delete_key", "--del_key", nargs=2, help="To delete the key")
    p.add_argument("-write_key", "--w_key", nargs=3, help="To write into key")
    p.add_argument("-find_key", "--f_key", nargs=2, help="To find the meaning in keys")
    args = p.parse_args()  # массив с аргументами, который находится, например, в write
    # print(args)
    if args.write:
        with open(args.write[1], "w") as file:
            file.write(args.write[0])
    if args.read:
        with open(args.read, "r") as file:
            mmm = file.read()
            print(mmm)
    if args.delete:
        try:
            os.remove(args.delete)
        except FileNotFoundError:
            print("No such file")
    if args.create:
        try:
            with open(args.create, "x") as f:
                print("File created")
        except FileExistsError:
            print("File is already exist")
    if args.rename:
        try:
            os.rename(args.rename[0], args.rename[1])
        except FileNotFoundError:
            print("No such file")
    if args.copy:
        try:
            shutil.copyfile(args.copy[0], args.copy[1] + '\\' + args.copy[0])
        except FileNotFoundError:
            print("No such file")
    if args.find:
        for root, dirs, files in os.walk(args.search[1]):
            for file in files:
                file_path = os.path.join(root, file)
                try:
                    with open(file_path, 'r') as f:
                        contents = f.read()
                        if args.search[0] in contents:
                            print(file_path)
                except Exception as e:
                    continue
    if args.del_key:
        key_val = args.del_key[1].replace(list(args.del_key[1].split("\\"))[0], "")[1:] + "\\" + args.del_key[0]
        path_root = my_choise(args.del_key[1])
        try:
            key = OpenKey(path_root, key_val)
            DeleteKey(path_root, key_val)
            CloseKey(key)
            print("Key deleted")
        except FileNotFoundError:
            print("No such key")
    if args.cr_key:
        key_val = args.cr_key[1].replace(list(args.cr_key[1].split("\\"))[0], "")[1:] + "\\" + args.cr_key[0]
        path_root = my_choise(args.cr_key[1])
        print(key_val)
        try:
            key = OpenKey(path_root, key_val)
            print("Key has already been created")
            CloseKey(key)
        except FileNotFoundError:
            key = CreateKey(path_root, key_val)
            CloseKey(key)
            print("Key created")
    if args.w_key:
        key_val = args.w_key[1].replace(list(args.w_key[1].split("\\"))[0], "")[1:] + "\\" + args.w_key[0]
        path_root = my_choise(args.w_key[1])
        try:
            key = OpenKey(path_root, key_val)
            SetValue(path_root, key_val, REG_SZ, args.w_key[2])
            CloseKey(key)
        except FileNotFoundError:
            key = CreateKey(path_root, key_val)
            SetValue(path_root, key_val, REG_SZ, args.w_key[2])
            CloseKey(key)
    if args.f_key:
        key_val = args.f_key[1].replace(list(args.f_key[1].split("\\"))[0], "")[1:]
        path_root = my_choise(args.f_key[1])
        search_registry(path_root, key_val, args.f_key[0])


if __name__ == '__main__':
    main()
