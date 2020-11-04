'''
Created on 29 Oct 2020

@author: julianporter
'''
import SoapySDR as SDR

class SoapyFlags(object):
    
    flagNames = {
        SDR.SOAPY_SDR_END_BURST : 'End Burst',
        SDR.SOAPY_SDR_HAS_TIME : 'Has valid timestamp',
        SDR.SOAPY_SDR_END_ABRUPT : 'Stream terminated prematurely',
        SDR.SOAPY_SDR_ONE_PACKET : 'Receive only a single packet',
        SDR.SOAPY_SDR_MORE_FRAGMENTS : 'More packet fragments to come',
        SDR.SOAPY_SDR_WAIT_TRIGGER : 'Wait for external trigger'
    }

    def __init__(self,flags):

        matched=0
        fs=[]
        names=[]
        for flag, name in self.flagNames.items():
            if flag & flags:
                matched |= flag
                fs.append(flag)
                names.append(name)

        self.flags=fs
        self.anomaly=matched ^ flags
        if self.anomaly != 0:
            names.append(f'anomalous flags {self.anomaly}')
        self.names=names

    def __str__(self):
        return ', '.join(self.names) if len(self.names)>0 else '<NONE>'
    
    def  __iter__(self):
        return iter(self.flags)
    
class SoapyError(Exception):
    
    def __init__(self,code,flags=0):
        super().__init__()
        try:
            self.code=code
            self.flags=SoapyFlags(flags)
            self.message=SDR.errToStr(code)
        except:
            self.message='Unknown error'
    
    def __str__(self):
        return f'SoapySDR error {self.code} : {self.message}. Flags: {self.flags}'
    def __repr__(self):
        return self.__str__()