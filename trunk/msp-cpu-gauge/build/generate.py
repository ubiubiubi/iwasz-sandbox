#!/usr/bin/env python

import numpy;
import math;
import sys

cnt = 1;
for i in numpy.arange (0, 2 * math.pi, math.pi / 256):
        sys.stdout.write (str (int (round (4095 * math.sin (i)))) + ",");
	
	if cnt % 20 == 0:
		sys.stdout.write ('\n');

	cnt += 1;

# COSINE
sys.stdout.write ('\n');
sys.stdout.write ('\n');
sys.stdout.write ('\n');

cnt = 1;
for i in numpy.arange (0, 2 * math.pi, math.pi / 256):
        sys.stdout.write (str (int (round (4095 * math.cos (i)))) + ",");
	
	if cnt % 20 == 0:
		sys.stdout.write ('\n');

	cnt += 1;

sys.stdout.write ('\n');

