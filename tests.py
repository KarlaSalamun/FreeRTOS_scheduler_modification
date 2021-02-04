#!/usr/bin/python3

from os import listdir 
from subprocess import call
import matplotlib.pyplot as plt 
import numpy as np
from operator import itemgetter

utils = []
t_edf = []
t_default = []
default_tuples = []
edf_tuples = []
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
			for step in range ( 90, 150, 10 ):
				if x_item > ( step/100 - 0.025 ) and x_item < ( step/100 + 0.025 ):
					x_item = step/100
					x_round.append( x_item )

	x_round = list( set( x_round ) )
	x_round.sort()

	return x_round

def compute_mean_data( x, tuples ):
	t_sum_1 = []
	t_sum_2 = []
	u_sum_1 = []
	u_sum_2 = []
	for x_val in x:
		tmp_sum_t_1 = 0
		tmp_sum_t_2 = 0
		tmp_sum_u_2 = 0
		tmp_sum_u_1 = 0
		cnt_1 = 0
		cnt_2 = 0
		for tuple in tuples:
			if tuple[0] > x_val - 0.05 and tuple[0] < x_val + 0.05:
				if tuple[0] < 0.95:
					tmp_sum_t_1 += tuple[1]
					tmp_sum_u_1 += tuple[2]
					cnt_1 = cnt_1+1
				else:
					tmp_sum_t_2 += tuple[1]
					tmp_sum_u_2 += tuple[2]
					cnt_2 = cnt_2+1
			else:
				if cnt_1 > 0 and x_val < 1:
					tmp_sum_t_1 = tmp_sum_t_1 / cnt_1
					tmp_sum_u_1 = tmp_sum_u_1 / cnt_1
					t_sum_1.append( tmp_sum_t_1 )
					u_sum_1.append( tmp_sum_u_1 )
					break
				elif cnt_2 > 0 and x_val > 1:
					tmp_sum_t_2 = tmp_sum_t_2 / cnt_2
					tmp_sum_u_2 = tmp_sum_u_2 / cnt_2
					t_sum_2.append( tmp_sum_t_2 )
					u_sum_2.append( tmp_sum_u_2 )
					break

	return t_sum_1, t_sum_2, u_sum_1, u_sum_2

################# PROGRAM BEGINS HERE #####################################

default_tuples, x = parse_file( 'outputs/default' )
edf_tuples, x = parse_file( 'outputs/edf' )
wspt_tuples, x = parse_file( 'outputs/wspt' )
edf_m_tuples, x = parse_file( 'outputs/edf_modif' )
wsrt_tuples, x = parse_file( 'outputs/wsrt' )

x_round = compute_mean_x( x )

t_sum_default_1, t_sum_default_2, u_sum_default_1, u_sum_default_2 = compute_mean_data( x_round, default_tuples )
t_sum_edf_1, t_sum_edf_2, u_sum_edf_1, u_sum_edf_2 = compute_mean_data( x_round, edf_tuples )
t_sum_wspt_1, t_sum_wspt_2, u_sum_wspt_1, u_sum_wspt_2 = compute_mean_data( x_round, wspt_tuples )
t_sum_edf_m_1, t_sum_edf_m_2, u_sum_edf_m_1, u_sum_edf_m_2 = compute_mean_data( x_round, edf_m_tuples )
t_sum_wsrt_1, t_sum_wsrt_2, u_sum_wsrt_1, u_sum_wsrt_2 = compute_mean_data( x_round, wsrt_tuples )

fig1 = plt.figure()
fig2 = plt.figure()
ax1 = fig1.add_subplot(121)
ax1.scatter( x_round[0:1], t_sum_default_1, label='default scheduler' )
ax1.scatter( x_round[0:1], t_sum_edf_1, label = 'EDF' )
ax1.scatter( x_round[0:1], t_sum_wspt_1, label = 'WSPT' )
ax1.scatter( x_round[0:1], t_sum_edf_m_1, label = 'WEDF' )
ax1.scatter( x_round[0:1], t_sum_wsrt_1, label = 'WSRT' )
ax1.plot( x_round[0:1], t_sum_default_1, label='default scheduler'  )
ax1.plot( x_round[0:1], t_sum_edf_1, label='EDF' )
ax1.plot( x_round[0:1], t_sum_wspt_1, label='WSPT' )
ax1.plot( x_round[0:1], t_sum_edf_m_1, label='WEDF' )
ax1.plot( x_round[0:1], t_sum_wsrt_1, label='WSRT' )
ax1.set_xlabel( 'utilization factor' )
ax1.set_ylabel( 'normalized weighted tardiness' )
ax2 = fig2.add_subplot(122)
ax2.scatter( x_round[2:6], u_sum_default_2, label='default scheduler' )
ax2.scatter( x_round[2:6], u_sum_edf_2, label='EDF' )
ax2.scatter( x_round[2:6], u_sum_wspt_2, label='WSPT' )
ax2.scatter( x_round[2:6], u_sum_edf_m_2, label='WEDF' )
ax2.scatter( x_round[2:6], u_sum_wsrt_2, label = 'WSRT' )
ax2.plot( x_round[2:6], u_sum_default_2, label='default scheduler' )
ax2.plot( x_round[2:6], u_sum_edf_2, label='EDF' )
ax2.plot( x_round[2:6], u_sum_wspt_2, label='WSPT' )
ax2.plot( x_round[2:6], u_sum_edf_m_2, label='WEDF' )
ax2.plot( x_round[2:6], u_sum_wsrt_2, label='WSRT' )
ax2.set_xlabel( 'utilization factor' )
ax2.set_ylabel( 'normalized number of late jobs' )
plt.legend()
plt.show()