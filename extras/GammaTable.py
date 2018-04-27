## Creates a gamma-corrected lookup table
#  Step 0 is always 0 (off)
#  Step 1 is always non-0 to get the biggest range possible
#  
import math
import sys

def gamma(nsteps, gamma):
  ##gammaedUp = [math.pow(x, gamma) for x in range(nsteps)]
  gammaedUp = [math.pow(x, gamma) for x in range(nsteps)]
  return [x/max(gammaedUp) for x in gammaedUp]

def rounder(topValue, gammas):
  return [min(topValue, round(x * topValue)) for x in gammas]
  ##return [min(topValue, math.ceil(x*topValue)) for x in gammas]
  
def countZero(values):
  count = 0
  for val in values:
    if(val == 0):
      count += 1
    else:
      return count

if __name__ == "__main__":
  myGamma = 2.3
  stepsIn = 101
  bitOut = 8
  varName = "myGammaTable"
  
  if(len(sys.argv) >= 4):
    myGamma = float(sys.argv[1])
    stepsIn = int(sys.argv[2])
    bitOut = int(sys.argv[3])
    
  if(len(sys.argv) >= 5):
    varName = sys.argv[4]
      
  
  output = open("gamma.h", "w")
  output.write("/* %d-step brightness table: gamma = %s for %d-bit PWM*/ \n" % (stepsIn, myGamma, bitOut))
  bitVar = int( (7 + bitOut) / 8) * 8
  output.write("const flvar_t %s[%d] PROGMEM = {\n\t" % (varName, stepsIn))
  stepsOut = int(math.pow(2, bitOut)) - 1
  
  values = rounder(stepsOut, gamma(stepsIn, myGamma))
  
  leadingZero = 0
  
  while countZero(values) > 1:
    leadingZero += countZero(values) - 1
    values = rounder(stepsOut, gamma(stepsIn + leadingZero, myGamma))
    values = values[leadingZero:]
  
  print(values)

  for index, value in enumerate(values, 1):
    output.write("%3i" % value)
    if(index != len(values)):
      if( index % 10 == 0):
        output.write(",\n\t")
      else:
        output.write(", ")
  output.write("\n};\n")
  output.close()