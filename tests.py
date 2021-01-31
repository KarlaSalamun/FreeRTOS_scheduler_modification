#!/usr/bin/python3

from os import listdir 
from subprocess import call
import matplotlib.pyplot as plt 
import numpy as np

# for file in listdir( 'inputs/' ):
#     print( file )
#     filename = "./build/exec " + file
#     call( filename, shell=True )

x = []
edf = []
edf_tuples = []

with open( 'outputs/results', 'r' ) as results:
	for row in results:
		entries = row.split()
		x.append( float( entries[0] ) )
		edf.append( float( entries[1] ) )
		edf_tuples.append( (float( entries[0] ), float( entries[1] )) );

sorted( edf_tuples, key = lambda x: x[0] )

x_round = []

for x_item in x:
	print( x_item )
	if x_item < 0.09 or x_item > 1.60:
		print( 'continue' )
		continue
	else:
		for step in range ( 90, 160, 5 ):
			if x_item > ( step/100 - 0.025 ) and x_item < ( step/100 + 0.025 ):
				x_item = step/100
				x_round.append( x_item )		


x_round = list( set( x_round ) )
x_round.sort()
print( x_round )

sum_edf = []
for x_val in x_round:
	print( x_val )
	tmp_sum = 0
	cnt = 0
	for tuple in edf_tuples:
		if tuple[0] > x_val - 0.025 and tuple[0] < x_val + 0.025:
			tmp_sum += tuple[1]
			cnt = cnt+1
		else:
			if cnt > 0:
				tmp_sum = tmp_sum / cnt
				sum_edf.append( tmp_sum )
				break

plt.scatter( x_round, sum_edf, label='EDF' )
plt.plot( x_round, sum_edf );
plt.show()