import sys
import re
import csv
import os

# 獲取傳遞的參數
param = sys.argv[1]
#param = "1"
# 印出參數
print("The parameter is: " + param)


logPowerPath = str(param)             #logPowerPath '/home/raspdslab/evaPoolPath/logTestPower.txt'


with open(logPowerPath, 'r') as f:
    lines = f.read().split('\n\n') # 以空白行分割內容
    last_second_content = lines[-2] # 抓取最後第二個內容


splitPathtxt     = logPowerPath.split("/")       # 使用 "/" 進行分割

splitPathtxt[-1] = "Sum" + splitPathtxt[-1]      # 在最後一個子字串前面加上 "Sum"

SumlogPowerPath  = "/".join(splitPathtxt)        # 重新連接字串    --- #SumlogPowerPath '/home/raspdslab/evaPoolPath/SumlogTestPower.txt'


# 將內容寫入其他檔案
with open(SumlogPowerPath, 'a') as f1:
    f1.write("\n")
    f1.write(param)
    f1.write("\n")
    f1.write(last_second_content)

    
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

# 每輪 Test Power 都將獲得一個平均值 ， 並將所有獲得之平均值，除以聚合的次數，獲得實驗結果
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
    # print(f"The average of numbers with remainder {key} is {avg_num}")


print(result)
print("Load Voltage:  {:6.3f} V".format(AVG[0]))
print("Current:       {:9.6f} A".format(AVG[1]))
print("Power:         {:6.3f} W".format(AVG[2]))
print("Percent:      {:3.1f}%"  .format(AVG[3]))
print("ALL_Power:  {:9.3f} W"   .format(AVG[4]))
print("AVG_Power:     {:6.3f} W".format(AVG[5]))
print("Count:         {:5.1f} T".format(AVG[6]))

# 建立一個對照表，將 key 換成新的值
new_keys = {'0': 'Load Voltage', '1': 'Current', '2': 'Power','3': 'Percent', '4': 'ALL_Power', '5': 'AVG_Power', '6': 'Count'}


print(AVG)
AVG["Load Voltage"] = AVG.pop(0)
AVG["Current"]      = AVG.pop(1)
AVG["Power"]        = AVG.pop(2)
AVG["Percent"]      = AVG.pop(3)
AVG["ALL_Power"]    = AVG.pop(4)
AVG["AVG_Power'"]   = AVG.pop(5)
AVG["Count"]        = AVG.pop(6)




# CSV檔案路徑和名稱
# SumlogPowerPathcsv = "/home/raspdslab/evaPoolPath/logTestPower.csv"


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
