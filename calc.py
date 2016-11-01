import numpy as np
from math import sqrt

infile = open("top5_data_valid.txt", "r")
content = infile.readlines()
infile.close()


averages = [x.split()[1:] for x in content if x[0] == 'A']
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance

averages = [x.split()[1:] for x in content if x[0] == 'R']
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance

averages = [x.split()[1:] for x in content if x[0:3] == "Pac"]
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance 

averages = [x.split()[1:] for x in content if x[0:3] == "Che"]
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance

averages = [x.split()[1:] for x in content if x[0:3] == "Pay"]
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance

averages = [x.split()[1:] for x in content if x[0] == 'L']
format_averages = []
for average in averages:
    format_averages.append(map(lambda x: float(x), average))
average_mean = []
for i in range(len(format_averages[0])):
    average_mean.append(sum([x[i] for x in format_averages])/len(format_averages))
print average_mean
average_variance = []
for i in range(len(format_averages[0])):
    average_variance.append(sqrt(np.var([x[i] for x in format_averages])))
print average_variance

avg = [float(x) for x in content if not x[0].isalpha()]
print sum(avg)/len(avg)
print sqrt(np.var(avg))
