#!/usr/bin/python3

from os import listdir 
from subprocess import call

for file in listdir( 'inputs/' ):
    print( file )
    filename = "./build/exec " + file
    call( filename, shell=True )