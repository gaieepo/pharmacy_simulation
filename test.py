infile = open("top5_data.txt", "r")
content = infile.readlines()
infile.close()
averages = [x for x in range(len(content)) if content[x][0] == 'A']

valid_averages = [x for x in averages if '0.000' not in content[x]]
valid_samples = [content[x:x+7] for x in valid_averages]
valid_content = reduce(lambda x, y: x + y, valid_samples)

outfile = open("top5_data_valid.txt", "w")
outfile.writelines(valid_content)
print len(valid_content)
print len(valid_content)/7
outfile.close()

