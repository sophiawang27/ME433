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

def fft(t, d, filtered=None, A=None, B=None):
    dt = 1.0/10000.0 # 10kHz
    data_pts = (str)(len(d))
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

    z = filtered # the data to make the fft from
    n = len(z) # length of the signal
    k = np.arange(n)
    T = n/Fs
    frq = k/T # two sides frequency range
    frq = frq[range(int(n/2))] # one side frequency range
    Z = np.fft.fft(z)/n # fft computing and normalization
    Z = Z[range(int(n/2))] # throw away repeated values

    fig, (ax1, ax2) = plt.subplots(2, 1)
    ax1.plot(t,y,'b')
    ax1.legend('Unfiltered Data')
    if (filtered != None):
        ax1.plot(t, filtered, 'r')
        ax1.legend('Unfiltered Data', 'Filtered Data')
    ax1.set_xlabel('Time')
    ax1.set_ylabel('Amplitude')
    ax1.set_title('Unfiltered and Filtered Signal D with Low Pass Sinc, 10Hz Cutoff, 39 Weights, bandwidth 48')
    # if (A != None):
        #ax1.set_title('Unfiltered and Filtered Signal D (A=%f, B=%f)' % (A, B))
    ax2.loglog(frq,abs(Y),'b') # plotting the fft
    ax2.legend('Unfiltered Data')
    if (filtered != None):
        ax2.plot(frq, abs(Z), 'r')
        ax2.legend('Unfiltered Data', 'Filtered Data')
    ax2.set_xlabel('Freq (Hz)')
    ax2.set_ylabel('|Y(freq)|')
    # if (A != None):
        #ax2.set_title('FFT of Signal D (A=%f, B=%f)' % (A, B))
    #ax2.set_title('FFT of Signal D averaging 50 data points')
    ax2.set_title('FFT of Signal D')
    plt.show()
    return Fs

# Write a loop that averages the last X number of data points and saves the result in a new list. 
# The new list will have X fewer data points, or you can assume the X data points before you started were 0. 
def maf(mafnum, t, d):
    mafA = []
    mafTA = []
    avg = 0
    for i in range(len(d)):
        if (i<mafnum):
            mafA.append(0)
        else:
            avg = (sum(d[(i-mafnum):i]))/mafnum
            mafA.append(avg)
    return t, mafA

# Write a loop that adds a value into the average with the two weights A and B, 
# so that new_average[i] = A * new_average[i-1] + B * signal[i]. 
# Note that A+B=1, otherwise the signal will get bigger with time (A+B>1) or go to 0 (A+B<1). 
def iir(t, d):
    A = 0.95
    B = 0.05
    iirA = []
    oldavg = 0 # previous average 
    for i in range(len(d)): # starts immediately at 0
        avg = A*oldavg + B*d[i]
        iirA.append(avg) #append to iirA
        oldavg = avg
    return t, iirA

#the sampling rate (A) is  10000.20000400008, cutoff 100, bandwidth 2000
#the sampling rate (B) is  3300.2000121219467, cutoff 33, bandwidth 400
#the sampling rate (C) is  2500.1250062503127, cutoff 25, bandwidth 300
#the sampling rate (D) is  400.0833506980621, cutoff 10, bandwidth 48
def fir(t, d):
    #coeffs
    h = [
    0.000000000000000000,
    0.000019658567897599,
    0.000125812352847421,
    0.000406645937045143,
    0.000979108648486075,
    0.001990768167858861,
    0.003613182590113233,
    0.006025493952257190,
    0.009389095003326116,
    0.013816464247975026,
    0.019339119053739771,
    0.025880670347927980,
    0.033240887006952040,
    0.041095417019080982,
    0.049013526889280339,
    0.056493286609061222,
    0.063010568696821173,
    0.068075618602019869,
    0.071289298924763003,
    0.072390754765093657,
    0.071289298924763003,
    0.068075618602019869,
    0.063010568696821201,
    0.056493286609061243,
    0.049013526889280346,
    0.041095417019080982,
    0.033240887006952026,
    0.025880670347927990,
    0.019339119053739782,
    0.013816464247975026,
    0.009389095003326124,
    0.006025493952257201,
    0.003613182590113236,
    0.001990768167858860,
    0.000979108648486073,
    0.000406645937045144,
    0.000125812352847421,
    0.000019658567897598,
    0.000000000000000000,
]
    firA = []
    pts = len(h)
    avg = 0
    for i in range(len(d)):
        if (i<pts):
            firA.append(0)
        else:
            vals = []
            for j in range(len(h)):
                vals.append(h[j]*d[i-j])
            avg = sum(vals)
            firA.append(avg)
    return t, firA



# simple plotting of fft
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

# maf
t, mafdata = maf(100, tA, dataA)
fft(tA, dataA, mafdata)
t, mafdata = maf(50, tB, dataB)
fft(tB, dataB, mafdata)
t, mafdata = maf(100, tA, dataC)
fft(tC, dataC, mafdata)
t, mafdata = maf(50, tD, dataD)
fft(tD, dataD, mafdata)

# iir
t, iirdata = iir(tA, dataA)
fft(tA, dataA, iirdata, 0.99, 0.01)
t, iirdata = iir(tB, dataB)
fft(tB, dataB, iirdata, 0.99, 0.01)
t, iirdata = iir(tC, dataC)
fft(tC, dataC, iirdata, 0.9, 0.1)
t, iirdata = iir(tD, dataD)
fft(tD, dataD, iirdata, 0.95, 0.05)

#fir
t, firdata = fir(tA, dataA)
fft(tA, dataA, firdata)
t, firdata = fir(tB, dataB)
fft(tB, dataB, firdata)
t, firdata = fir(tC, dataC)
fft(tC, dataC, firdata)
t, firdata = fir(tD, dataD)
fft(tD, dataD, firdata)