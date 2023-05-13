import os, time
import numpy as np

tick = 1

np.set_printoptions(threshold=np.inf)

I = np.full((30, 100)," ")
#I[0,:] = '#'
#I[-1,:] = '#'
#I[:,0] = '#'
#I[:,-1] = '#'
I[10,:] = "-"
I[19,:] = "-"
I[11:18,36] = "_"
I[11:18,37] = "_"
I[11:18,38] = "_"
I[11:18,39] = "_"
I[11:18,-6] = "_"
I[11:18,-7] = "_"
I[11:18,-8] = "_"
I[11:18,-9] = "_"

def printScreen():
  for l in I:
    print("".join(l))

def refresh():
  os.system('clear')
  printScreen()

def placeScreen(O, l, c):
  I[l:O.shape[0]+l, c:O.shape[1]+c] = O

def empty(n):
  if n == 1:
    placeScreen(np.full((4, 21)," "), 3, 3)
  elif n == 2:
    placeScreen(np.full((4, 21)," "), 24, 3)
  elif n == 3:
    placeScreen(np.full((4, 21)," "), 3, 58)
  elif n == 4:
    placeScreen(np.full((4, 21)," "), 24, 58)
  refresh()

def ride(extended):
  time.sleep(tick)
  placeScreen(car, 13, 3)
  refresh()
  time.sleep(tick)
  placeScreen(np.full((4, 21)," "), 13, 3)
  refresh()
  if extended == 1:
    placeScreen(car, 13, 58)
    refresh()
    time.sleep(tick)
    placeScreen(np.full((4, 21)," "), 13, 58)
    refresh()

def park(n):
  if n == 1:
    ride(0)
    placeScreen(car, 3, 3)
  elif n == 2:
    ride(0)
    placeScreen(car, 24, 3)
  elif n == 3:
    ride(1)
    placeScreen(car, 3, 58)
  elif n == 4:
    ride(1)
    placeScreen(car, 24, 58)
  refresh()

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

#(4, 21)
car = np.array([
[' ',' ',' ',' ',' ',' ','.','-','-','-','-','-','-','-','-','.',' ',' ',' ',' ',' '],
[' ','_','_','_','_','/','_','_','_','_','_','|','_','_','_',' ','\\','_','_','_',' '],
['O',' ',' ',' ',' ','_',' ',' ',' ','-',' ','|',' ',' ',' ','_',' ',' ',' ',',','*'],
[' ','\'','-','-','(','_',')','-','-','-','-','-','-','-','(','_',')','-','-','\'',' ']
])

#(8, 27)
parking = np.full((8, 27)," ")
parking[0,:] = '#'
parking[-1,:] = '#'
parking[:,0] = '#'
parking[:,-1] = '#'



placeScreen(parking, 1, 0)
placeScreen(parking, 22, 0)
placeScreen(parking, 1, 55)
placeScreen(parking, 22, 55)

placeScreen(pump, 0, 31)
placeScreen(pump, 21, 31)
placeScreen(pump, 0, 86)
placeScreen(pump, 21, 86)

placeScreen(np.array([['1','0','0','%']]), 2, 12)
placeScreen(np.array([['1','0','0','%']]), 23, 12)
placeScreen(np.array([['1','0','0','%']]), 2, 67)
placeScreen(np.array([['1','0','0','%']]), 23, 67)

park(1)
park(2)
park(3)
park(4)

ride(1)
ride(1)

empty(1)
empty(2)
empty(3)
empty(4)


#for i in range(1):
#  printScreen(I)
#  time.sleep(0.1)
#  os.system('clear')