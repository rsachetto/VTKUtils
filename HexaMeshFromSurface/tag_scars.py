from sys import argv

scar_file = open(argv[1], 'r')

for line in scar_file:
    l = line.split(',')

    x = float(l[0])
    y = float(l[1])
    z = float(l[2])
    id = int(4)

    if x > 79300 and (id == 1 or id ==2):
        print line.strip() + ",b"
    elif if (id == 1 or id == 2):
        print line.strip() + ",s"
    else:
        print line.strip()
