import csv

def save(SavePath, SaveData):
	with open(SavePath, 'w', newline='') as CSVFile:
		writer = csv.writer(CSVFile)
		writer.writerow(["日期", "姓名", "类别", "单价", "数量", "赠送数量", "折扣", "邮费", "总价"])
		for i in range(len(SaveData["Name"])):
			writer.writerow([SaveData["Date"][i], SaveData["Name"][i], SaveData["Type"][i], SaveData["Price"][i], 
							 SaveData["Amount"][i], SaveData["Deal"][i], SaveData["Discount"][i], SaveData["Post"][i], SaveData["Total"][i]])

def read(ReadPath):
	RetData = {"Date": [], "Name": [], "Type": [], "Price": [], "Amount": [], "Deal": [], "Discount": [], "Post": [], "Total": []}
	i = 0
	with open(ReadPath, 'r', newline='') as CSVFile:
		reader = csv.reader(CSVFile)
		for row in reader:
			if i == 0:
				i += 1
				continue
			RetData["Date"].append(row[0])
			RetData["Name"].append(row[1])
			RetData["Type"].append(row[2])
			RetData["Price"].append(row[3])
			RetData["Amount"].append(row[4])
			RetData["Deal"].append(row[5])
			RetData["Discount"].append(row[6])
			RetData["Post"].append(row[7])
			RetData["Total"].append(row[8])
	return RetData

def read_outcome(ReadPath):
	RetData = {"Date": [], "Type": [], "Price": []}
	i = 0
	with open(ReadPath, 'r', newline='') as CSVFile:
		reader = csv.reader(CSVFile)
		for row in reader:
			if i == 0:
				i += 1
				continue
			RetData["Date"].append(row[0])
			RetData["Type"].append(row[2])
			RetData["Price"].append(row[3])
	return RetData