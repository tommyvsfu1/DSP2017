
mapping = dict()
fin = open("Big5-ZhuYin.map", 'r', encoding='big5-hkscs')
fout = open("ZhuYin-Big5.map", 'w', encoding='big5-hkscs')

for line in fin:

	char = line[0]
	fout.write(char + '\t' + char + '\n')

	poly = line[2:].split('/')
	for phone in poly:
		if phone[0] in mapping:
			if not char in mapping[phone[0]]:
				mapping[phone[0]].append(char)
		else:
			dict = []
			dict.append(char)
			mapping[phone[0]] = dict


for k, v in mapping.items():
	line = k + "\t"
	for word in v:
		line = line + word + " "
	fout.write(line + '\n')