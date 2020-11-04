#!/usr/bin/env python3

import SoapySDR
import sys
from soapy import SoapyError, Receiver

            

centreFreq = float(sys.argv[1])*1.0e6
bandwidth = float(sys.argv[2])*1.0e6

print(f'Centre frequency is {centreFreq}Hz with bandwidth {bandwidth}Hz')

#enumerate devices
results = Receiver.receivers()
for index, result in enumerate(results): print(f'Receiver {index} : {result}')

#create device instance
rx=Receiver(frequency=centreFreq,bandwidth=bandwidth)

#query device info
info = rx.info
print(str(info))

rx.connect()

#receive some samples
for i in range(10):
    try:
        response = rx.get()
        print(f'Got {len(response.data)} samples at time {response.timestamp} with flags {response.flags}') 
    except SoapyError as e:
        print(e)
    except Exception as e:
        print(e)

rx.disconnect()


