fg = open("/home/cheng/fctree/CaseStudy/homo_k_1_lmd_5_pathlmd.txt", 'r')

gene = list()

cnt = 0
for line in fg:
    cnt += 1
    if cnt == 1:
        continue
    x = line.rstrip()
    x = int(x)
    gene.append(x)

fhomo = open("/home/cheng/fctree/CaseStudy/homo/l6.txt", "r")
fres = open("/home/cheng/fctree/CaseStudy/res/l6.txt", "w")

for line in fhomo:
    x, y = line.rstrip().split(" ")
    x, y = int(x), int(y)
    if (x in gene) and (y in gene):
        fres.write(line)
