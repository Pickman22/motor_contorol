import serial
import threading
import binascii
import time
import struct
import argparse
import json
import matplotlib.pyplot as plt
import logging
import random
import os


STOPSTR = 'STOP'
logging.basicConfig(level = logging.DEBUG)

class DummyPort():

    def __init__(self, n = 100):
        self._N = n
        self._n = 0

    def __exit__(self, *args):
        pass

    def __enter__(self, *args):
        pass

    def readline(self):
        if self._n >= self._N:
            self._n = 0
            ret = STOPSTR
        else:
            self._n += 1
            ret = str(random.random())
        return ret + '\n'

    def open(self):
        return True

    def close(self):
        return True

def ReadLog(fname):
    try:
        fp = open(fname, 'r+')
    except Exception as e:
        print('Something went wrong: {}'.format(e))
        exit()
    with fp:
        rawdata = fp.readlines()[0]
        return json.loads(rawdata)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description = 'Serial Port Monitor', )
    parser.add_argument('-p', '--port', action = 'store', dest = 'port', default = '/dev/ttyACM0')
    parser.add_argument('-b', '--baudrate', action = 'store', dest = 'baudrate', default = 115200, type = int)
    parser.add_argument('-f', '--fname', action='store', dest='fname', default=None, help='Log file name')
    parser.add_argument('-t', '--timeout', action='store', dest='timeout', default='5.', help='Port timeout ms', type=float)
    parser.add_argument('-l', '--log', action = 'store', dest='log_data', default=1, help='Log data', type=bool)
    parser.add_argument('-n', '--runs', action='store', dest='runs', default=1, help='Number of iterations', type=int)
    parser.add_argument('-d', '--debug', action='store_true', dest='debug', default=0)

    args = parser.parse_args()

    if args.debug:
        logging.getLogger().setLevel(logging.DEBUG)

    port = None
    tmp_a = []
    data_d = {}
    labels = []

    if(args.runs < 1):
        raise ValueError('Argument runs must be positive.')

    try:
        if not args.debug:
            port = serial.Serial(args.port, args.baudrate, timeout = args.timeout)
        else:
            port = DummyPort()
    except Exception as e:
        if port:
            port.close()
        print('Something happened!: {}'.format(e))
    else:

        print '####################################################################'
        print '######################### El Robotista #############################'
        print '####################################################################'
        print '########### Receive Motor Data Through Serial Port V0.1 ############'
        print '####################################################################'

        for i in range(args.runs):
            with port:
                while True:
                    data = port.readline()[:-1]
                    if not data:
                        logging.debug('Timeout.')
                        break
                    elif data == STOPSTR:
                        logging.debug('STOPSTR.')
                        break
                    else:
                        logging.debug('Data received: {}'.format(data))
                        tmp_a.append(float(data))
                if tmp_a:
                    data_d[str(i)] = tmp_a
                    plt.plot(tmp_a)
                    labels.append('Run: {}'.format(i))
                    logging.debug('Run complete: {}'.format(tmp_a))
                    tmp_a = []


        if args.log_data and data_d:
            path_created = False


            if not args.fname:
                fname = 'datalog_' + str(int(time.time()))  + '.txt'
            else:
                fname = args.fname
            try:
                if not os.path.exists('datalog'):
                    logging.debug('Creating datalog directory...')
                    os.makedirs('datalog')
                with open('datalog/' + fname, 'w+') as flog:
                    logging.debug('Logging data to: {}'.format(fname))
                    flog.write(json.dumps(data_d))
                    logging.debug('Logged data: {}'.format(data_d))
            except Exception as e:
                print('Something is wrong: {}'.format(e))
                
    plt.minorticks_on()
    plt.grid(which='both')
    plt.legend(labels)
    plt.ylabel('Current [mA]')
    plt.xlabel('Sample [n * Ts]')
    plt.show()
    print('Bye.')
