# -*- coding: utf-8 -*-
import subprocess
import time
import sys
import os

def get_memory_usage(process_name):
    #command  = f"ps aux | grep '{process_name}' | awk '{{ total += $6 }} END {{ printf (\"%d\\n\",total)}}'"
    command  = "ps aux | grep '{0}' | awk '{{ total += $6 }} END {{ printf (\"%d\\n\",total)}}'".format(process_name)
    output   = subprocess.check_output(command, shell=True)
    return int(output.decode().strip())


file_path = '../evaPoolPath/threadName.txt'  # ´À´«¬°¹ê»ÚªºÀÉ®×¸ô®|

with open(file_path, "r") as file:
    # Åª¨úÀÉ®×¤º®e
    file_content = file.read()



process_name = file_content     ### client 端的 main 執行檔案
logPath      = sys.argv[1]         ### "/home/bn10513137/evaPoolPath/logTrainMemory.txt"

while True:
    memory_usage = get_memory_usage(process_name)
    with open(logPath, "a") as f1:
        f1.write(str(memory_usage))
        f1.write("\n")
