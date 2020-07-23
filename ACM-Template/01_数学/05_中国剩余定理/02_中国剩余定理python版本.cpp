def gcd(a, b):
    if b == 0:
        return a
    else:
        return gcd(b, a % b)

def exgcd(a, b, x, y):
    re, tmp = 0, 0
    if b == 0:
        return a, 1, 0
    re, x, y = exgcd(b, a % b, x, y)
    tmp = x
    x = y
    y = tmp - (a//b)*y
    return re, x, y


def inv(a, b):
    x, y, r = 0, 0, 0
    r, x, y = exgcd(a, b, x, y)
    while x < 0:
        x += b
    return x


def excrt(n, M, C):
    M1, M2, C1, C2, T = 0, 0, 0, 0, 0
    for i in range(2, n+1):
        M1, M2, C1, C2 = M[i-1], M[i], C[i-1], C[i]
        T = gcd(M1, M2)
        if (C2-C1) % T != 0:
            return 1e13
        M[i] = (M1 * M2) // T
        C[i] = (inv(M1//T, M2//T) * (C2-C1) // T) % (M2 // T) * M1 + C1
        C[i] = (mod[i] % M[i] + M[i]) % M[i]
        if C[i] > 1e12:
            return 1e13
    return C[n]