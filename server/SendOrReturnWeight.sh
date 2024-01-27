#!/bin/bash


# 路徑檔案名稱
file="shellPath.txt"

# 創建陣列以存儲讀取到的每行內容
declare -a lines
while IFS= read -r line; do
    # 將每行的內容添加到陣列中
    lines+=("$line")
done < "$file"

# 分配陣列中最後一行的內容給不同的變數
eval "SERVER=${lines[0]}"
eval "RASP=${lines[1]}"
eval "NANO=${lines[2]}"
# 定義要搜尋的字串
eval "search_string_1=${lines[3]}"
eval "search_string_2=${lines[4]}"
eval "search_string_3=${lines[5]}"

# 輸出每個變數的內容
echo "SERVER         : $SERVER"
echo "RASP           : $RASP"
echo "NANO           : $NANO"
echo "search_string_1: $search_string_1"
echo "search_string_2: $search_string_2"
echo "search_string_3: $search_string_3"

# 將換行符號去除
SERVER_fixed=$(echo "$SERVER" | tr -d '\r')
RASP_fixed=$(echo "$RASP" | tr -d '\r')
NANO_fixed=$(echo "$NANO" | tr -d '\r')
searchString1=$(echo "$search_string_1" | tr -d '\r')
searchString2=$(echo "$search_string_2" | tr -d '\r')
searchString3=$(echo "$search_string_3" | tr -d '\r')


# 定義來源及目錄
#SERVER_1="/mnt/c/Server專案檔路徑/weightPoolPath/"
#RASP_1="raspdslab@100.82.153.59:Rasp專案檔名稱/weightPoolPath/"
#NANO_1="bn10513137@100.66.82.83:NANO專案檔名稱/weightPoolPath/"



# 定義要搜索的字符串
#search_string_1="SERVER_SEND_TRAIN_TO_NODE"
#search_string_2="NODE_SEND_WEIGHT_TO_SERVER"
#search_string_3="SERVER_SEND_AGGRE_TO_NODE"

# 從 Server 端傳遞檔案至 Client 端指令如下(將本地目錄同步到遠程主機：)
# sshpass -p wsl設定密碼 rsync -avzhP <Server來源資料夾> <Client帳號名稱@Client IP位址:Client目標資料夾>
# 程式上(帶變數進去縮減版)
# sshpass -p 6517 rsync -avzhP $SERVER_1 $RASP_1
# 程式上(未縮減版)
# sshpass -p 6517 rsync -avzhP /mnt/c/Users/Dslab/Desktop/demo1/ServerDemo1/ raspdslab@100.82.153.59:RaspDemo1/weightPoolPath/

# 從 Client 端取回檔案至 Server 端指令如下(從遠程主機同步文件到本地目錄)
# sshpass -p wsl設定密碼 rsync -avzhP <Client帳號名稱@Client IP位址:Client來源資料夾> <Server目標資料夾>
# 程式上(帶變數進去縮減版)
# sshpass -p 6517 rsync -avzhP $NANO_1 $SERVER_1
# 程式上(未縮減版)
# sshpass -p 6517 rsync -avzhP bn10513137@100.66.82.83:/NanoDemo1/weightPoolPath/ /mnt/c/論文方向/SDH_CCODE/Main_SDH/SHARE_DIR/ 

> SEND_FILE_CTRL_MSG.txt # 先清除檔案內容

# 如果文件存在且不為空
while true; do
	if [ -f SEND_FILE_CTRL_MSG.txt ]; then
		echo "找到檔案"
	  # 使用 grep 命令搜索文件
		if grep $searchString1 SEND_FILE_CTRL_MSG.txt; then      #CTRL_MSG_1
			echo "SERVER傳DATA到NODE端"
			echo "傳送檔案"
			sshpass -p 6517 rsync -avzhP $SERVER_fixed $NANO_fixed
			sshpass -p 6517 rsync -avzhP $SERVER_fixed $RASP_fixed
			sleep 2
			> SEND_FILE_CTRL_MSG.txt # 清除檔案內容
		elif grep $searchString2 SEND_FILE_CTRL_MSG.txt; then    #CTRL_MSG_2
			echo "NODE傳WEIGHT到SERVER端"
			echo "傳回檔案"
			sshpass -p 6517 rsync -avzhP $NANO_fixed $SERVER_fixed
			sshpass -p 6517 rsync -avzhP $RASP_fixed $SERVER_fixed
			sleep 2
			> SEND_FILE_CTRL_MSG.txt # 清除檔案內容
		elif grep $search_string_3 SEND_FILE_CTRL_MSG.txt; then   #CTRL_MSG_3
			echo "SERVER端傳聚合WEIGHT到NODE端"
			echo "傳送聚合檔案"
			sshpass -p 6517 rsync -avzhP $SERVER_fixed $NANO_fixed
			sshpass -p 6517 rsync -avzhP $SERVER_fixed $RASP_fixed
			sleep 2
			> SEND_FILE_CTRL_MSG.txt # 清除檔案內容
		else
			echo "字符串未找到"
		fi
	else	
 	  echo "文件不存在或為空"
	fi
	sleep 1
done










