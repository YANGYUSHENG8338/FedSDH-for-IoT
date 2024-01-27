# -*- coding: utf-8 -*-
import subprocess
import time
import sys
import os

def run_command(command):
    logPath = sys.argv[1]
    p = subprocess.Popen(command, shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
    while True:
        line = p.stdout.readline().decode().strip()
        if not line:
            break
        #print(line)
        with open(logPath, "a") as f:
            f.write(line + "\n")

while True:
    run_command("echo '6517' | sudo -S cat /sys/bus/i2c/drivers/ina3221x/6-0040/iio:device0/in_power?_input")


