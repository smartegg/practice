#!/usr/bin/python
#this file simply read the current directory 
#and build a Makfile  based on the .h .cc files
#(0)./gen_makefile.py  generate a Makefile 
#(1)make app              
import os
import re

path = './'
listing = os.listdir(path)
fdout = open('./Makefile', 'w+')#trunc this file

all_objs = ''
fdout.write('app : $(OBJ)\n')
fdout.write('\t g++ -o $@ *.o -g\n')

for filename in listing:
    if (filename.endswith('.cc')):
	obj = filename[:-3] + '.o'
	fdout.write(obj + ': ')
	all_objs = all_objs + obj + ' '
	with open(os.path.join(path, filename), 'r') as fdin:
	    for line in fdin:
		match = re.search(r'"\w+[.]h"', line)
		if match:
		    header =  match.group()
		    fdout.write(header[1:-1] + ' ')
	fdin.close()
	fdout.write(filename)
	fdout.write('\n\t')
	fdout.write('g++ -o ' + obj + ' -c ' + filename + ' -g \n')
#a ungly impl here
fdout.close()
fdout = open('./Makefile', 'r+')#trunc this file
os.lseek(fdout.fileno(), 0, os.SEEK_SET)
lines = fdout.read();
os.lseek(fdout.fileno(), 0, os.SEEK_SET)
fdout.write('OBJ = ' + all_objs + '\n\n')
fdout.write(lines);
fdout.close()
