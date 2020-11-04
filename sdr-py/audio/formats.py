'''
Created on 2 Nov 2020

@author: julianporter
'''
import sounddevice
import numpy as np



class PCMFormat(object):
    
    def __init__(self,name,minimum,maximum):
        self.name=name
        self.min=minimum
        self.max=maximum
        
        self.a=2.0/(maximum-minimum)
        self.b=-(maximum+minimum)/(maximum-minimum)
        
        self.divisor=np.max(np.abs([self.min,self.max]))
        
    def __str__(self):
        return self.name
    
    def __call__(self,values):
        return np.clip(values,self.min,self.max)*self.a - self.b
    

class PCMStreamCharacteristics(object):
    
    FORMATS = {
        'uint8': PCMFormat('uint8',0,255),
        'int8' : PCMFormat('int8',-128,127),
        'int16': PCMFormat('int16',-32768,32767),
        'int32': PCMFormat('int32',-4294967296,4294967295),
        'float': PCMFormat('float',-1,1)
    }
    
    def __init__(self,rate=48000,fmt='int16',blocksize=64):
        self.rate=rate
        self.dtype=fmt
        self.blocksize=blocksize
        self.format=self.FORMATS.get(self.dtype)
        
    def check(self,dev):
        sounddevice.check_input_settings(device=dev.index, dtype=self.dtype, samplerate=self.rate)
        
    
