from Crypto.Util.number import getPrime, bytes_to_long
flag = b'PolyCTF{n0_r3us3_k3y_1n_OTP!}'

FLAG = bytes_to_long(flag)

p = getPrime(1024)
q = getPrime(116)
print(p)
print(q)
n = p * q
print(hex(n))
e = 2**16 + 1
ct = pow(FLAG, e, n)

print(f'n: {n}')
print(f'e: {e + p}')
print(f'ct: {ct}')
#
#
