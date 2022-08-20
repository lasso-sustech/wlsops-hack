#!/usr/bin/env python3
import sys
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

def execute(command, args):
    with MmapContext() as ctx:
        if args.reset:
            reset_tx_params(ctx)
        else:
            acq = [0,1,2,3] if args.ac=='all' else [int(args.ac)]
            ret = [ctx.set_tx_params(ac, args.aifs[0], args.cw_min[0], args.cw_min[0]) for ac in acq]
            print(ret)
    pass

def main():
    parser = argparse.ArgumentParser(description='wlsctrl for wlsops_hack.')
    subparsers = parser.add_subparsers(dest='command')
    ##
    ac_set = subparsers.add_parser('set', help='set specific AC queue EDCA parameters.')
    ac_set.add_argument('ac',     type=str, help='AC queue index, "all" to choose all.')
    ac_set.add_argument('aifs',   type=int, help='0 - 255')
    ac_set.add_argument('cw_min', type=int, help='0 - 65535')
    ac_set.add_argument('cw_max', type=int, help='0 - 65535')
    ##
    ac_reset = subparsers.add_parser('reset', help='reset specific AC queue with default EDCA parameters.')
    ac_reset.add_argument('ac',   type=str, help='AC queue index, "all" to choose all.')
    ##
    args = parser.parse_args()
    execute(args.command, args)
    pass

if __name__=='__main__':
    try:
        main()
    except Exception as e:
        raise e
    finally:
        pass
