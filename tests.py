#!/usr/bin/python3

from os import listdir 
from subprocess import call
import matplotlib.pyplot as plt 
import numpy as np
from operator import itemgetter

utils = []
t_edf = []
t_edf_tuples = []
t_default = []
default_tuples = []
u_edf = []
u_edf_tuples = []
u_default = []

def parse_file( filename ):
	x = []
	t = []
	u = []
	tuples = []
	with open( filename, 'r' ) as res_file:
		for row in res_file:
			entries = row.split()
			mean_proctime = float( entries[4] )
			num_jobs = float( entries[3] )
			mean_weight = float( entries[5] )
			entries[1] = float( entries[1] ) / ( num_jobs * mean_proctime * mean_weight )
			entries[6] = float( entries[6] ) / ( num_jobs * mean_weight * mean_weight )
			x.append( float( entries[0] ) )
			t.append( float( entries[1] ) )
			u.append( float( entries[6] ) )
			tuples.append( (float( entries[0] ), float( entries[1] ), float( entries[6] )) )

			tuples.sort(key = itemgetter(0))

	return tuples, x

def compute_mean_x( x ):
	x_round = []
	for x_item in x:
		if x_item < 0.09 or x_item > 1.60:
			continue
		else:
			for step in range ( 90, 150, 5 ):
				if x_item > ( step/100 - 0.025 ) and x_item < ( step/100 + 0.025 ):
					x_item = step/100
					x_round.append( x_item )

	x_round = list( set( x_round ) )
	x_round.sort()

	return x_round

def compute_mean_data( x, tuples ):
	t_sum = []
	u_sum = []
	for x_val in x:
		tmp_sum_t = 0
		tmp_sum_u = 0
		cnt = 0
		for tuple in tuples:
			if tuple[0] > x_val - 0.025 and tuple[0] < x_val + 0.025:
				tmp_sum_t += tuple[1]
				tmp_sum_u += tuple[2]
				cnt = cnt+1
			else:
				if cnt > 0:
					tmp_sum_t = tmp_sum_t / cnt
					tmp_sum_u = tmp_sum_u / cnt
					t_sum.append( tmp_sum_t )
					u_sum.append( tmp_sum_u )
					break

	return t_sum, u_sum

################# PROGRAM BEGINS HERE #####################################

for file in listdir( 'inputs/' ):
    print( file )
    filename = "./build/exec " + file
    call( filename, shell=True )

default_tuples, x = parse_file( 'outputs/default' )

x_round = compute_mean_x( x )

t_sum_default, u_sum_default = compute_mean_data( x_round, default_tuples )

fig = plt.figure()
ax1 = fig.add_subplot(211)
ax1.scatter( x_round, t_sum_default, label='default scheduler' )
ax1.plot( x_round, t_sum_default )
ax1.set_xlabel( 'utilization factor' )
ax1.set_ylabel( 'normalized weighted tardiness' )
ax2 = fig.add_subplot(212)
ax2.scatter( x_round, u_sum_default, label='default scheduler' )
ax2.plot( x_round, u_sum_default )
ax2.set_xlabel( 'utilization factor' )
ax2.set_ylabel( 'normalized number of late jobs' )
plt.legend()
plt.show()