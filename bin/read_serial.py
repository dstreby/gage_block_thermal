#!/usr/bin/env python

import sys, getopt, os
import serial

def main(argv):
  inputDev = None
  outputFile = None
  try:
    opts, args = getopt.getopt(argv,"hi:o:",["idev=","ofile="])
  except getopt.GetoptError:
    showUsage()
    sys.exit(2)
  for opt, arg in opts:
    if opt == '-h':
      showUsage()
      sys.exit()
    elif opt in ("-i", "--idev"):
      inputDev = arg
    elif opt in ("-o", "--ofile"):
      outputFile = arg

  if (inputDev is None or outputFile is None):
    print 'Missing required arguments!'
    showUsage()
    sys.exit(1)
  else:
    print 'Input device is "', inputDev
    print 'Output file is "', outputFile
    readSerial(inputDev, outputFile)

def showUsage():
  print("Usage:")
  print(os.path.basename(__file__) + " -i <inputDev> -o <outputfile>")

def readSerial(inputDev, outputFile):
  ser = serial.Serial(inputDev, 9600)
  f = open(outputFile, 'w')
  try:
    print('Starting serial capture, use ctrl+c to break...')
    while True:
      serial_value = ser.readline().rstrip()
      f.write(serial_value + '\n')
      print(serial_value)
      sys.stdout.flush()
  except KeyboardInterrupt:
      print('\n' + 'Exiting.')
      f.close()

if __name__ == "__main__":
   main(sys.argv[1:])

