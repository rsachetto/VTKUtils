from sys import argv

scar_file = open(argv[1], 'r')

for line in scar_file:
    l = line.split(',')

    x = float(l[0])
    y = float(l[1])
    z = float(l[2])
    id = int(4)

    if x >= 80800 and x<= 81600 and y>=95200 and y<=96000 and z>=43200 and z<=44000:
        pass
    else:
        print line.strip()
