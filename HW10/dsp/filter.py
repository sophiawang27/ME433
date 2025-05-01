import csv
import matplotlib.pyplot as plt # for plotting
import numpy as np # for sine function


def opencsv(filename):
    t = [] # column 0
    data1 = [] # column 1

    with open(filename) as f:
        # open the csv file
        reader = csv.reader(f)
        for row in reader:
            # read the rows 1 one by one
            t.append(float(row[0])) # leftmost column
            data1.append(float(row[1])) # second column

    for i in range(len(t)):
        # print the data to verify it was read
        #print(str(t[i]) + ", " + str(data1[i]))
        continue
    return t, data1

def plotdata(t, data1):
    plt.plot(t,data1,'b-*')
    plt.xlabel('Time [s]')
    plt.ylabel('Signal')
    plt.title('Signal vs Time')
    plt.show()


def maf(mafnum, t, d):
    mafA = []
    mafTA = []
    avg = 0
    for i in range(mafnum, len(d)):
        avg = sum(d[i-mafnum])
    # need first ten data points before starting


def iir():
    print("hello")# starts immediately at 0
    # previous average 
    # new average
    #append to iirA


def fir():
    print("hi")# need floating point coefficients


tA, dataA = opencsv('sigA.csv')
plotdata(tA, dataA)
tB, dataB = opencsv('sigB.csv')
plotdata(tB, dataB)
tC, dataC = opencsv('sigC.csv')
plotdata(tC, dataC)
tD, dataD = opencsv('sigD.csv')
plotdata(tD, dataD)