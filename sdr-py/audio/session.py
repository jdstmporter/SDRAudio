'''
Created on 2 Nov 2020

@author: julianporter
'''
import sounddevice
import numpy as np
from .device import PCMDeviceSpecification
from .formats import PCMStreamCharacteristics
from util import SYSLOG

class PCMSessionDelegate(object):
    
    def __call__(self,data):
        pass
    
    def connect(self,samplerate):
        pass
    
    def startListeners(self):
        pass
    
    def stopListeners(self):
        pass

class PCMSessionHandler(object):
    def __init__(self,delegate=PCMSessionDelegate()):
        self.pcm=None
        self.format=None
        self.delegate=delegate
        
    def connect(self,dev):
        try:
            if self.pcm:
                self.stop()
            self.pcm=PCMInputSession(dev,delegate=self.delegate)
            self.delegate.connect(self.pcm.samplerate)          
        except Exception as ex:
            SYSLOG.error(f'Error connecting to {dev} : {ex}')
            
    def disconnect(self):
        try:
            if self.pcm:
                self.stop()
                self.pcm=None
        except Exception as ex:
            SYSLOG.error(f'Error disconnecting from {self.pcm} : {ex}')
            
    def start(self):
        self.delegate.startListeners()
        self.pcm.start()
        self.format=self.pcm.format
        SYSLOG.info(f'Started {self.pcm}')
        
        
    def stop(self):
        if self.pcm:
            self.pcm.stop()
            self.format=None
        self.delegate.stopListeners()
    
    
class PCMOutputSession(object):
    
    def __init__(self,specification : PCMDeviceSpecification):
        self.specification=specification
        self.device=str(specification)
        self.name=specification.name
        self.index=specification.index  
        self.pcm = None
        self.format=None  
        
    def start(self,characteristics = PCMStreamCharacteristics()):
        characteristics.check(self.specification)
        self.format=characteristics.format
       
        self.pcm=sounddevice.OutputStream(samplerate=characteristics.rate,blocksize=characteristics.blocksize,device=self.index,
                                            dtype=characteristics.dtype,callback=self.callback)
        self.pcm.start()
        
    def stop(self):
        if self.pcm: self.pcm.stop(True)
        self.pcm=None
       
    def kill(self):
        if self.pcm: self.pcm.abort(True)
        self.pcm=None
        
    def write(self,buffer):
        self.pcm.write(buffer)        
        
    @property
    def samplerate(self):
        return self.pcm.samplerate  
    
    @property
    def active(self):
        if self.pcm==None: return None
        return self.pcm.active 
    
    def callback(self,data,frames,time,status):
        if status:
            SYSLOG.info(f'{status}; wrote {frames} frames')
  

class PCMInputSession(object):
     
    def __init__(self,specification : PCMDeviceSpecification, delegate : PCMSessionHandler = PCMSessionHandler()):
        self.specification=specification
        self.device=str(specification)
        self.name=specification.name
        self.index=specification.index
        self.delegate=delegate
        self.pcm = None
        self.format=None
        
    
 
        
        
    @property
    def samplerate(self):
        return self.pcm.samplerate
 
    def callback(self,indata,frames,time,status):
        if status:
            SYSLOG.info(f'{status} but got {frames} frames')
        if len(indata)>0:
            self.delegate(np.mean(indata,axis=1)/self.format.divisor)


    @property
    def active(self):
        if self.pcm==None: return None
        return self.pcm.active        
        
    def start(self,characteristics = PCMStreamCharacteristics()):
        characteristics.check(self.specification)
        self.format=characteristics.format
       
        self.pcm=sounddevice.InputStream(samplerate=characteristics.rate,blocksize=characteristics.blocksize,device=self.index,
                                            dtype=characteristics.dtype,callback=self.callback)
        self.pcm.start()
    
    
    def stop(self):
        if self.pcm: self.pcm.stop(True)
        self.pcm=None
       
    def kill(self):
        if self.pcm: self.pcm.abort(True)
        self.pcm=None
     
