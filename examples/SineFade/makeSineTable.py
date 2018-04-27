import math

biggestStep = 90
resolutionMax = 255

for x in range(0, biggestStep + 1):
  y = int(round(math.sin(x / (2 * biggestStep) * math.pi) * resolutionMax, 0))
  print("%3i" % y, end = '')
  if x != biggestStep:
    print(', ', end = '')
  if (x % 10) == 9:
    print()
print()