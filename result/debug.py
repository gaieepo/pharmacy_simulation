infile = open("debug.txt", "r")
outfile = open("sorted.txt", "w")
content = [x[3:].split(':') for x in infile.readlines() if x[0] == 'N']
content = [[int(x[0]), x[1]] for x in content]
infile.close()

content.sort(key = lambda x : (x[0], float(x[1].split()[1])))
content = [":".join([str(x[0]),x[1]]) for x in content]
print content
outfile.writelines(content)
outfile.close()

