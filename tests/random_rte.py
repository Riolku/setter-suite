from random import randint

arr = sorted(map(int, input().split()))

if randint(1, 500) == 1:
    raise RuntimeError
else:
    print(*arr)
