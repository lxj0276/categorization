#!/usr/bin/env python
#-*- coding: utf-8 -*-
import textclassfy
import ctypes
from ctypes import *
import os
import sys
reload(sys)
import re
if __name__=="__main__":
	#soo = CDLL("/home/seanxywang/git/categorization/textclassfy.so")
	#soo.init()
	fin = open(sys.argv[1],'r')
	fout = open(sys.argv[2],'a')
	print "ok"
	#
	print "ok"
	for line in fin:
		scrstr = line.replace("\n",'').replace(" ",'')
		print "python" + scrstr
		linestr = textclassfy.Categorizate(scrstr)
		print "python" + linestr
		fout.write(linestr)