import os
import numpy as np

np.set_printoptions(threshold=np.inf)

S = np.full((29, 149)," ")
S[10,:90] = "-"
S[18,:90] = "-"
S[11:17,33:37] = "_"
S[11:17,78:82] = "_"
S[:,90] = "|"
S[:,148] = "|"

#(9, 14)
pump = np.array([
  [' ',' ',' ','.','-','\"','\"','\"','\"','-','.',' ',' ',' '],
  [' ',' ',' ','|','=','=',' ',' ','=','=','|','-','.',' '],
  [' ',' ',' ','|','~','~',' ','~','~','~','|','`','\\','\\'],
  [' ',' ',' ','|','L','I','L','I','L','I','|',' ','|','|'],
  [' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|','/','/',' '],
  [' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|','/',' ',' '],
  [' ',' ',' ','|',' ',' ',' ',' ',' ',' ','|',' ',' ',' '],
  [' ','_','_','|','_','_','_','_','_','_','|','_','_',' '],
  ['[','_','_','_','_','_','_','_','_','_','_','_','_',']']
])

#(5, 21)
car = np.array([
[' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '],
[' ',' ',' ',' ',' ',' ','.','-','-','-','-','-','-','-','-','.',' ',' ',' ',' ',' '],
[' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '],
['O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'],
[' ','\'','-','-','(','_',')','-','-','-','-','-','-','-','(','_',')','-','-','\'',' ']
])

P = np.array([
['#','#','#','#','#','#','#'],
['#','#',' ',' ',' ','#','#'],
['#','#','#','#','#','#','#'],
['#','#',' ',' ',' ',' ',' '],
['#','#',' ',' ',' ',' ',' ']
])

#(9, 27)
parking_pump = np.full((9, 27),"#")
parking_pump[1:-1,1:-1] = " "

#(9, 27)
parking = np.full((9, 27),"+")
parking[1:-1,1:-1] = " "

def printScreen():
  for l in S:
    print("".join(l))

def placeScreen(O, y, x):
  S[y:O.shape[0]+y, x:O.shape[1]+x] = O

cfx1 = 3
cfx2 = 48
cpx1 = 95
cpx2 = 123
cy1 = 2
cy2 = 22
cry = 12

def refresh():
  os.system('clear')
  printScreen()
  S[10,:90] = "-"
  S[18,:90] = "-"
  S[11:17,33:37] = "_"
  S[11:17,78:82] = "_"
  S[:,90] = "|"
  S[:,148] = "|"
  placeScreen(P, 12, 116)

placeScreen(parking_pump, 0, 0)
placeScreen(parking_pump, 20, 0)
placeScreen(parking_pump, 0, 45)
placeScreen(parking_pump, 20, 45)

placeScreen(pump, 0, 28)
placeScreen(pump, 20, 28)
placeScreen(pump, 0, 73)
placeScreen(pump, 20, 73)

placeScreen(parking, 0, 92)
placeScreen(parking, 20, 92)
placeScreen(parking, 0, 120)
placeScreen(parking, 20, 120)
placeScreen(P, 12, 116)

def showLevel(tank):
  C = car.copy()
  level = str(tank) + '%'
  if tank < 10:
    level = '  ' + level
  elif tank < 100:
    level = ' ' + level
  C[0, 9:13] = np.array(list(level)) 
  return C

def ride(x, tank):                                  #3, 48, 95, 123
  placeScreen(showLevel(tank), cry, x)
  refresh()

def emptyRide(x):                                   #3, 48, 95, 123
  placeScreen(np.full(car.shape," "), cry, x)
  refresh()

def park(n, tank):                                  #1 a 4 - bombas, 5 a 8 - vagas
  if n == 1:
    placeScreen(showLevel(tank), cy1, cfx1)
  elif n == 2:
    placeScreen(showLevel(tank), cy2, cfx1)
  elif n == 3:
    placeScreen(showLevel(tank), cy1, cfx2)
  elif n == 4:
    placeScreen(showLevel(tank), cy2, cfx2)
  elif n == 5:
    placeScreen(showLevel(tank), cy1, cpx1)
  elif n == 6:
    placeScreen(showLevel(tank), cy2, cpx1)
  elif n == 7:
    placeScreen(showLevel(tank), cy1, cpx2)
  elif n == 8:
    placeScreen(showLevel(tank), cy2, cpx2)
  refresh()

def empty(n):
  if n == 1:
    placeScreen(np.full(car.shape," "), cy1, cfx1)
  elif n == 2:
    placeScreen(np.full(car.shape," "), cy2, cfx1)
  elif n == 3:
    placeScreen(np.full(car.shape," "), cy1, cfx2)
  elif n == 4:
    placeScreen(np.full(car.shape," "), cy2, cfx2)
  elif n == 5:
    placeScreen(np.full(car.shape," "), cy1, cpx1)
  elif n == 6:
    placeScreen(np.full(car.shape," "), cy2, cpx1)
  elif n == 7:
    placeScreen(np.full(car.shape," "), cy1, cpx2)
  elif n == 8:
    placeScreen(np.full(car.shape," "), cy2, cpx2)
  refresh()
