import sys
import re
import csv
import os

# 獲取傳遞的參數
param = sys.argv[1]
# param = "1"
# 印出參數
print("The parameter is: " + param)

logMemoryPath = str(param)       #logMemoryPath '/home/raspdslab/evaPoolPath/logTestMemory.txt'
# 開啟文件並讀取數據
with open(logMemoryPath, 'r') as f:
    lines = f.read().split('\n') # 以空白行分割內容
    last_second_content = lines[-2] # 抓取最後第二個內容




# 將數據轉換為浮點數列表
data = [float(line.strip()) for line in lines if line.strip()]

# 計算總和和平均值
total = sum(data)
average = total / len(data)

# 輸出結果
print(f"Total: {total}")
print(f"Average: {average}")
print(f"Number of values: {len(data)}")

#index          12345678901234567890123456789 
#logMemoryPath '/home/raspdslab/evaPoolPath/logTestMemory.txt'

splitPathtxt      = logMemoryPath.split("/")       # 使用 "/" 進行分割

splitPathtxt[-1]  = "Sum" + splitPathtxt[-1]       # 在最後一個子字串前面加上 "Sum"

SumlogMemoryPath  = "/".join(splitPathtxt)         # 重新連接字串    --- #SumlogMemoryPath '/home/raspdslab/evaPoolPath/SumlogTestMemory.txt'

print(SumlogMemoryPath)

# 將內容寫入其他檔案
with open(SumlogMemoryPath, 'a') as f1:
    print(f"Total: {total}",file=f1)
    print(f"Average: {average}",file=f1)
    print(f"Number of values: {len(data)}",file=f1)





 
# 使用正則表達式匹配每一行中的數值
pattern = r':\s*(-?\d+\.\d+|-?\d+)' # 匹配冒號後的數值，可以是浮點數或整數


# pattern = r':\s*(\d+\.\d+|\d+)'  
values = []  # 儲存匹配的數值
with open(SumlogMemoryPath, 'r') as f2:
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

# 每 Test Memory 獲得一個平均值 ， 並將所有獲得之平均值，除以聚合的次數，獲得實驗結果

# print(result)
for i, val in enumerate(values):
    remainder = i % 3  # 取得索引值的餘數
    if remainder not in result:
        result[remainder] = val
        count[remainder] = 1
    else:
        result[remainder] += val
        count[remainder] += 1

for key, value in result.items():
    avg_num = value / count[key]
    AVG[key] = avg_num


# print(result)
print("Total All_device Memory:  {:9.6f} KB".format(AVG[0]))
print("AVG   All_device Memory:  {:9.6f} KB".format(AVG[1]))
print("Count:         {:5.1f} T".format(AVG[2]))




# # 讀取舊的 csv 檔案，存成列表

# print(AVG)
AVG["Total All_device Memory"] = AVG.pop(0)
AVG["AVG   All_device Memory"] = AVG.pop(1)
AVG["Count"] = AVG.pop(2)

print(AVG)



# # 透過迴圈將每個字典的 key 取代成新的值


# CSV檔案路徑和名稱
# SumlogMemoryPathcsv = "/home/raspdslab/evaPoolPath/logTestMemory.csv"

splitPathcsv        = SumlogMemoryPath.split("/")  # 使用 "/" 進行分割

splitPathcsv[-1]    = splitPathcsv[-1].replace(".txt", ".csv")  # 將最後一個子字串的 "txt" 替換為 "csv"

SumlogMemoryPathcsv = "/".join(splitPathcsv)  # 重新連接字符串


# 設定CSV檔案的寫入模式為'append'，即在文件末尾添加新的內容
with open(SumlogMemoryPathcsv, mode='a', newline='') as csvfile:
    fieldnames = AVG.keys()  # 取得要寫入的字典的所有鍵
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

    # 如果檔案是空的，寫入標題欄位
    if csvfile.tell() == 0:
        writer.writeheader()

    writer.writerow(AVG)  # 寫入字典資料
