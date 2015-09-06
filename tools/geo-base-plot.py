#!/usr/bin/python

import matplotlib.pyplot as plt
from sys import stdin
from argparse import ArgumentParser

parser = ArgumentParser()
parser.add_argument("--scatter", required=False, default="False")

args = parser.parse_args()

for l in stdin:
    l = l.strip()[1:-1].split(',')
    l.append(l[0])
    print "Points count =", len(l) - 1
    x = []
    y = []
    for s in l:
        s = s.strip()[1:-1].split(' ')
        s[0], s[1] = s[1], s[0]
        x.append(float(s[0]))
        y.append(float(s[1]))
    plt.plot(x, y)
    if args.scatter == "True":
        plt.scatter(x, y, alpha = 0.2)

plt.show()
