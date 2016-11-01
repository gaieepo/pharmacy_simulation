infile = open("debug.txt", "r")
outfile = open("resorted.txt", "w")
content = [x.split() for x in infile.readlines() if x[0] == 't']
infile.close()

content.sort(key = lambda x : (int(x[1]), float(x[4])))
outfile.writelines([" ".join(x)+'\n' for x in content])
outfile.close()

