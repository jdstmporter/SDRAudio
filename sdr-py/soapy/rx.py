'''
Created on 30 Oct 2020

@author: julianporter
'''

import numpy
import SoapySDR
import collections
from SoapySDR import SOAPY_SDR_RX, SOAPY_SDR_CF32
from .flags import SoapyError, SoapyFlags

SoapyData = collections.namedtuple('SoapyData', ['data','flags','timestamp'])

def range2str(r):
    return ' - '.join([f'{x} Hz' for x in r])

def dict2str(d):
    return '\n'.join([f'{k} = {v}' for k,v in d.items()])
    
    

class ChannelInfo(object):
    
    def get(self,method,*args):
        meth = getattr(self.sdr,method)
        return meth(SOAPY_SDR_RX, self.channel,*args)
    
    def getDict(self,listMethod,getMethod):
        items=self.get(listMethod)
        out=collections.OrderedDict()
        for item in items:
            out[item] = self.get(getMethod,item)
        return out    
        
    
    def __init__(self,sdr,channel):
        self.sdr=sdr
        self.channel=channel
        self.info = self.get('getChannelInfo')
        self.antennae = list(self.get('listAntennas'))
        self.bandwidths = list(self.get('getBandwidthRange'))
        self.gains = self.getDict('listGains','getGain')
        self.freqRanges = self.getDict('listFrequencies','getFrequencyRange')
        self.hasDCOffset = self.get('hasDCOffsetMode')
        
    def __str__(self):
        ranges = ', '.join([f'{k} : {range2str(v)}' for k,v in self.freqRanges.items()])
        gains = 'dB , '.join([f'{k} : {v}' for k,v in self.gains.items()])
        bws = ', '.join([f'{x} Hz' for x in self.bandwidths])
        ant = ', '.join([f'{x}' for x in self.antennae])
        return '\n'.join([
            f'Channel number : {self.channel}',
            f'Antennae : {ant}',
            f'Bandwidths : {bws}',
            f'Gains : {gains}',
            f'Ranges : {ranges}',
            f'DC offset available: {self.hasDCOffset}',
            f'General Info',
            dict2str(self.info)
        ])
        
                                         
class ReceiverInfo(object):
    
    def __init__(self,sdr):
        self.sdr=sdr
        self.nChannels=self.sdr.getNumChannels(SOAPY_SDR_RX)
        self.channels=[ChannelInfo(self.sdr,n) for n in range(self.nChannels)]
        self.info=self.sdr.getHardwareInfo()
        self.name=self.sdr.getHardwareKey()
        
    def __len__(self):
        return self.nChannels
    
    def __getitem__(self,n):
        return self.channels[n]   
    
    def __str__(self): 
        s = '\n'.join([str(c) for c in self.channels]) 
        return f'General info for {self.name}:\n{dict2str(self.info)}\nChannels\n{s}'
            
        
    
        
        

class Receiver(object):
    
    def __init__(self,channel=0,rate=1.0e6,frequency=433.0e6,bandwidth=1.0e6):
        self.channel=channel
        self.rate=rate
        self.bandwidth=bandwidth
        self.freq=frequency
        
        self.rx=None
        self.buffer=numpy.array([0]*1024, numpy.complex64)
        
        args = dict(driver="rtlsdr")
        self.sdr = SoapySDR.Device(args)
        self._update()
        
    def _update(self):
        self.sdr.setSampleRate(SOAPY_SDR_RX, self.channel, self.rate)
        self.sdr.setFrequency(SOAPY_SDR_RX, self.channel, self.freq)
        self.sdr.setBandwidth(SOAPY_SDR_RX,self.channel, self.bandwidth)
 
    def connect(self):
        self.rx = self.sdr.setupStream(SOAPY_SDR_RX, SOAPY_SDR_CF32)
        self.sdr.activateStream(self.rx) 
        
    def disconnect(self):
        if self.rx:
            self.sdr.deactivateStream(self.rx)
            self.sdr.closeStream(self.rx)
            self.rx=None
        
    def tune(self,frequency,bandwidth=None):
        flag = self.rx is not None
        self.disconnect()
        
        if bandwidth is None:
            bandwidth = min(frequency,self.rate-frequency)
        self.freq=frequency
        self.bandwidth=bandwidth
        self._update()
        
        if flag:
            self.connect()
            
    def get(self):
            sr = self.sdr.readStream(self.rx, [self.buffer], len(self.buffer))
            if sr.ret<0:
                raise SoapyError(sr.ret,sr.flags)
            return SoapyData(data=self.buffer,flags=SoapyFlags(sr.flags),timestamp=sr.timeNs)
        
        

        
     
    @property
    def info(self):       
        return ReceiverInfo(self.sdr)
    @property
    def gains(self):
        return self.sdr.listGains(SOAPY_SDR_RX, self.channel)
    @property
    def bandwidths(self):
        return self.sdr.listBandwidths(SOAPY_SDR_RX,self.channel)
    @property
    def frequencyRange(self):
        return self.sdr.getFrequencyRange(SOAPY_SDR_RX, self.channel)
    
    @classmethod
    def receivers(cls):
        return SoapySDR.Device.enumerate()