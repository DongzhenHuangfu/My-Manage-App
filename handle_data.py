import csv

def save(SavePath, SaveData):
	with open(SavePath, 'w', newline='') as CSVFile:
		writer = csv.writer(CSVFile)
		writer.writerow(["日期", "姓名", "类别", "单价", "数量", "赠送数量", "邮费", "总价"])
		for i in range(len(SaveData["Name"])):
			writer.writerow([SaveData["Date"][i], SaveData["Name"][i], SaveData["Type"][i], SaveData["Price"][i], 
							 SaveData["Amount"][i], SaveData["Deal"][i], SaveData["Post"][i], SaveData["Total"][i]])
