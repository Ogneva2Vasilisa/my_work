import gmpy2

a = -21476282882
m = 25045948061219400671
b = 2161489875

inv_a = gmpy2.invert(a, m)  # Находим обратный элемент

if inv_a is not None:
    x = (inv_a * b) % m
    print(f"x ≡ {x} (mod {m})")
    print(pow(17, x, 50091896122438801343), 4)
else:
    print("Обратного элемента не существует.")


