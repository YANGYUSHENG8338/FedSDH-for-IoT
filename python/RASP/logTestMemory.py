# -*- coding: utf-8 -*-
import subprocess
import time
import sys
import os

def get_memory_usage(process_name):
    command  = f"ps aux | grep '{process_name}' | awk '{{ total += $6 }} END {{ printf (\"%d\\n\",total)}}'"
    output   = subprocess.check_output(command, shell=True)
    return int(output.decode().strip())

# 打開文本檔案
file_path = "../evaPoolPath/threadName.txt"  # 替換為實際的檔案路徑

with open(file_path, "r") as file:
    # 讀取檔案內容
    file_content = file.read()

# 將讀取到的字串賦值給變數
process_name = file_content        ### client 端的 main 執行檔案
logPath      = sys.argv[1]         ### '/home/raspdslab/evaPoolPath/logTestMemory.txt'

while True:
    memory_usage = get_memory_usage(process_name)
    with open(logPath, "a") as f1:
        f1.write(str(memory_usage))
        f1.write("\n")
