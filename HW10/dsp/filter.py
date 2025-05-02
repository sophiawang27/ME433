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

def fft(t, d):
    dt = 1.0/10000.0 # 10kHz
# sample rate = number of data points / total time of samples
    Fs = len(d)/t[-1] # sample rate
    Ts = 1.0/Fs; # sampling interval
    ts = np.arange(0,t[-1],Ts) # time vector
    y = d # the data to make the fft from
    n = len(y) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Y = np.fft.fft(y)/n # fft computing and normalization
    Y = Y[range(int(n/2))] # throw away repeated values

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t,y,'b')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax2.loglog(frq,abs(Y),'b') # plotting the fft
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    plt.show()
    return Fs

def maf(mafnum, t, d):
    mafA = []
    mafTA = []
    avg = 0
    for i in range(mafnum, len(d)):
        avg = (sum(d[i-mafnum:i]))/mafnum
        mafTA.append(avg)
        
    return t[mafnum:], mafTA


def iir(t, d):
    iirA = []
    oldavg = 0 # previous average 
    for i in range(len(d)): # starts immediately at 0
        avg = 0.5*sum(d[i] + oldavg) # new average
        iirA.append(avg) #append to iirA
        oldavg = avg
    return t, iirA



def fir():
    print("hi")# need floating point coefficients


tA, dataA = opencsv('sigA.csv')
samp_rate = fft(tA, dataA)
print("the sampling rate is ", samp_rate)
tB, dataB = opencsv('sigB.csv')
samp_rate = fft(tB, dataB)
print("the sampling rate is ", samp_rate)
tC, dataC = opencsv('sigC.csv')
samp_rate = fft(tC, dataC)
print("the sampling rate is ", samp_rate)
tD, dataD = opencsv('sigD.csv')
samp_rate = fft(tD, dataD)
print("the sampling rate is ", samp_rate)
#plotdata(tA, dataA)
# tB, dataB = opencsv('sigB.csv')
# plotdata(tB, dataB)
# tC, dataC = opencsv('sigC.csv')
# plotdata(tC, dataC)
# tD, dataD = opencsv('sigD.csv')
# plotdata(tD, dataD)

# t, mafdata = maf(10, tA, dataA)
