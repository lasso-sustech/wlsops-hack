from wlsctrl.wlsctrl import set_tx_params, MmapContext,reset_tx_params
import argparse

def main(args):
    if args.reset:
        try:
            with MmapContext() as ctx:
                reset_tx_params(ctx)
                print("reset")
        except Exception as e:
            raise e
    else:
        try:
            with MmapContext() as ctx:
                set_tx_params(ctx, [args.ac], args.aifs, args.cw_min, args.cw_max)
                print("Set ac%d: cw_min=%d, cw_max=%d, aifs=%d" % (args.ac, args.cw_min, args.cw_max, args.aifs))
        except Exception as e:
            raise e
    


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--reset", action="store_true", help="reset edca parameters")
    parser.add_argument("--ac", type=int, help="ac index", default= 3)
    parser.add_argument("--cw-min", type=int, help="cw-min", default=-1)
    parser.add_argument("--cw-max", type=int, help="cw-max", default=-1)
    parser.add_argument("--aifs", type=int, help="aifs", default=-1)

    args = parser.parse_args()
    main(args)

