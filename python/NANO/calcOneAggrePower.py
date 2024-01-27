# -*- coding: utf-8 -*-
import sys
import re
import csv


# 獲取傳遞的參數
param = sys.argv[1]
#param = "1"
# 印出參數
print("The parameter is: " + param)


logPowerPath = str(param)             #logPowerPath '/home/bn10513137/evaPoolPath/logOneAggrePower.txt'

with open(logPowerPath, 'r') as f:
    contents = f.read()
content = contents.replace("[sudo] password for bn10513137: ", "")
numbers = re.findall(r'\d+', content)
lines = [int(num) for num in numbers]









sums   = {}
counts = {}

# 計算這輪聚合輪次紀錄的平均功耗
for i, num in enumerate(lines):
    key = i % 3
    if key not in sums:
        sums[key] = 0
        counts[key] = 0
    sums[key] += num
    counts[key] += 1
print(sums)
sorted_sums = sorted(sums.items(), key=lambda x: x[1], reverse=True)
print(sorted_sums)
count_number = round(len(numbers) / 3)
print(count_number)


splitPathtxt     = logPowerPath.split("/")       # 使用 "/" 進行分割

splitPathtxt[-1] = "Sum" + splitPathtxt[-1]      # 在最後一個子字串前面加上 "Sum"

SumlogPowerPath  = "/".join(splitPathtxt)        # 重新連接字串    --- #SumlogPowerPath '/home/bn10513137/evaPoolPath/SumlogTestPower.txt'


# 將內容寫入其他檔案
with open(SumlogPowerPath, 'a') as f1:
    print(f'Total All_device Power : {sorted_sums[0][1]} mW',file=f1)
    print(f'AVG   All_device Power : {sorted_sums[0][1] / counts[0]} mW',file=f1)
    print(f'Total CPU_device Power : {sorted_sums[1][1]} mW',file=f1)
    print(f'AVG   CPU_device Power : {sorted_sums[1][1] / counts[1]} mW',file=f1)
    print(f'Total GPU_device Power : {sorted_sums[2][1]} mW',file=f1)
    print(f'AVG   GPU_device Power : {sorted_sums[2][1] / counts[2]} mW',file=f1)
    print(f'Count Power : {count_number} Times',file=f1)







    
# 使用正則表達式匹配每一行中的數值
pattern = r':\s*(-?\d+\.\d+|-?\d+)' # 匹配冒號後的數值，可以是浮點數或整數


# pattern = r':\s*(\d+\.\d+|\d+)'  
values = []  # 儲存匹配的數值
with open(SumlogPowerPath, 'r') as f2:
    content = f2.read()
for line in content.splitlines():
    match = re.search(pattern, line)
    if match:
        value = float(match.group(1))  # 將匹配到的數值轉換為浮點數
        values.append(value)

print(values)  # 輸出儲存的數值列表

result = {}
count  = {}
AVG    = {}

# 每輪 OneAggre Power 都將獲得一個平均值 ， 並將所有獲得之平均值，除以聚合的次數，獲得實驗結果
# print(result)
# 透過迴圈將每個字典的 key 取代成新的值
for i, val in enumerate(values):
    remainder = i % 7  # 取得索引值的餘數
    if remainder not in result:
        result[remainder] = val
        count[remainder] = 1
    else:
        result[remainder] += val
        count[remainder] += 1

for key, value in result.items():
    avg_num = value / count[key]
    AVG[key] = avg_num



print(result)
print("Total All_device Power:  {:9.6f} mW".format(AVG[0]))
print("AVG   All_device Power:  {:9.6f} mW".format(AVG[1]))
print("Total CPU_device Power:  {:9.6f} mW".format(AVG[2]))
print("AVG   CPU_device Power:  {:9.6f} mW".format(AVG[3]))
print("Total GPU_device Power:  {:9.6f} mW".format(AVG[4]))
print("AVG   GPU_device Power:  {:9.6f} mW".format(AVG[5]))
print("Count Power:         	{:5.1f} Times".format(AVG[6]))


AVG["Total All_device Power"] = AVG.pop(0)
AVG["AVG   All_device Power"] = AVG.pop(1)
AVG["Total CPU_device Power"] = AVG.pop(2)
AVG["AVG   CPU_device Power"] = AVG.pop(3)
AVG["Total GPU_device Power"] = AVG.pop(4)
AVG["AVG   GPU_device Power"] = AVG.pop(5)
AVG["Count Power"] = AVG.pop(6)

print(AVG)




# CSV檔案路徑和名稱
# SumlogPowerPathcsv = "/home/bn10513137/evaPoolPath/SumlogOneAggrePower.csv"


splitPathcsv       = SumlogPowerPath.split("/")  # 使用 "/" 進行分割

splitPathcsv[-1]   = splitPathcsv[-1].replace(".txt", ".csv")  # 將最後一個子字串的 "txt" 替換為 "csv"

SumlogPowerPathcsv = "/".join(splitPathcsv)  # 重新連接字符串

# 設定CSV檔案的寫入模式為'append'，即在文件末尾添加新的內容
with open(SumlogPowerPathcsv, mode='a', newline='') as csvfile:
    fieldnames = AVG.keys()  # 取得要寫入的字典的所有鍵
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

    # 如果檔案是空的，寫入標題欄位
    if csvfile.tell() == 0:
        writer.writeheader()

    writer.writerow(AVG)  # 寫入字典資料

