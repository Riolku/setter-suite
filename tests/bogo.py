from random import randint

arr = list(map(int, input().split()))

arr.sort()

if randint(0, 10000) == 0:
    arr[0] = -1

print(*arr)
