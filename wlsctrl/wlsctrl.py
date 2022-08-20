#!/usr/bin/env python3
import ctypes
import argparse
from pathlib import Path

ROOT = Path(__file__).parent.resolve()

class MmapContext:
    def __init__(self) -> None:
        self.lib = ctypes.CDLL( str(ROOT/'libwlsctrl.so') )
        pass
    
    def __enter__(self):
        self.lib.w_init()
        return self.lib
    
    def __exit__(self, exc_type, exc_value, exc_tb):
        self.lib.w_fini()
    pass

def reset_tx_params(ctx):
    ctx.set_tx_params(0, 2, 3,  7)
    ctx.set_tx_params(1, 2, 7,  15)
    ctx.set_tx_params(2, 3, 15, 1023)
    ctx.set_tx_params(3, 7, 15, 1023)
    pass

def main():
    with MmapContext() as ctx:
        ctx.set_tx_params(1, 1, 1, 1)
    pass

if __name__=='__main__':
    try:
        main()
    except Exception as e:
        raise e
    finally:
        pass
