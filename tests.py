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
			entries[6] = float( entries[6] ) / ( num_jobs * mean_proctime * mean_weight )
			x.append( float( entries[0] ) )
			t.append( float( entries[1] ) )
			u.append( float( entries[6] ) )
			tuples.append( (float( entries[0] ), float( entries[1] ), float( entries[6] )) )

			tuples.sort(key = itemgetter(0))

	return tuples, x


def compute_mean_data( x, tuples ):
	t_sum = []
	u_sum = []
	for x_val in x:
		tmp_sum_t = 0
		tmp_sum_u = 0
		cnt = 0
		for tuple in tuples:
			if tuple[0] > x_val - 0.1 and tuple[0] <= x_val:
				tmp_sum_t += tuple[1]
				tmp_sum_u += tuple[2]
				cnt = cnt + 1
			else:
				if cnt > 0:
					tmp_sum_t = tmp_sum_t / cnt
					tmp_sum_u = tmp_sum_u / cnt
					t_sum.append( tmp_sum_t )
					u_sum.append( tmp_sum_u )
					break

	return t_sum, u_sum

################# PROGRAM BEGINS HERE #####################################

default_tuples, x = parse_file( 'outputs/default' )
edf_tuples, x = parse_file( 'outputs/edf' )
wspt_tuples, x = parse_file( 'outputs/wspt' )
wsrt_tuples, x = parse_file( 'outputs/wsrt' )
rms_tuples, x = parse_file( 'outputs/rms' )
lst_tuples, x = parse_file( 'outputs/lst' )
wedf_tuples, x = parse_file( 'outputs/wedf' )

x_round = [0.1 * x for x in range(6, 15+1)]

t_sum_default, u_sum_default = compute_mean_data( x_round, default_tuples )
t_sum_edf, u_sum_edf = compute_mean_data( x_round, edf_tuples )
t_sum_wspt, u_sum_wspt = compute_mean_data( x_round, wspt_tuples )
t_sum_wsrt, u_sum_wsrt = compute_mean_data( x_round, wsrt_tuples )
t_sum_rms, u_sum_rms = compute_mean_data( x_round, rms_tuples )
t_sum_lst, u_sum_lst = compute_mean_data( x_round, lst_tuples )
t_sum_wedf, u_sum_wedf = compute_mean_data( x_round, wedf_tuples )

f1 = plt.figure(1)

plt.plot( x_round[0:5], t_sum_default[0:5], label='static scheduler, $\pi_i = w_i$', marker='s', markersize=8 )
plt.plot( x_round[0:5], t_sum_edf[0:5], label = 'EDF', marker='^', markersize=8 )
plt.plot( x_round[0:5], t_sum_wspt[0:5], label = 'WSPT', marker='v', markersize=8 )
plt.plot( x_round[0:5], t_sum_wedf[0:5], label = '$\pi_{ij} = d_{ij} / w_{i}$', marker='o', markersize=8 )
plt.plot( x_round[0:5], t_sum_wsrt[0:5], label = 'WSRT', marker='X', markersize=8 )
plt.plot( x_round[0:5], t_sum_rms[0:5], label = 'RMS', marker='D', markersize=8 )
plt.plot( x_round[0:5], t_sum_lst[0:5], label = 'LST', marker='*', markersize=8 )

plt.xlabel( 'utilization factor $U$' )
plt.ylabel( 'normalized weighted tardiness $\overline{T_w}$' )
plt.grid()
plt.legend()

f1.savefig( "twt1.pdf", bbox_inches='tight' )

f2 = plt.figure(2)

plt.plot( x_round[4:10], t_sum_default[4:10], label='static scheduler, $\pi_i = w_i$', marker='s', markersize=8 )
plt.plot( x_round[4:10], t_sum_edf[4:10], label = 'EDF', marker='^', markersize=8 )
plt.plot( x_round[4:10], t_sum_wspt[4:10], label = 'WSPT', marker='v', markersize=8 )
plt.plot( x_round[4:10], t_sum_wedf[4:10], label = '$\pi_{ij} = d_{ij} / w_{i}$', marker='o', markersize=8 )
plt.plot( x_round[4:10], t_sum_wsrt[4:10], label = 'WSRT', marker='X', markersize=8 )
plt.plot( x_round[4:10], t_sum_rms[4:10], label = 'RMS', marker='D', markersize=8 )
plt.plot( x_round[4:10], t_sum_lst[4:10], label = 'LST', marker='*', markersize=8 )

plt.xlabel( 'utilization factor $U$' )
plt.ylabel( 'normalized weighted tardiness $\overline{T_w}$' )
plt.grid()
plt.legend()

f2.savefig( "twt2.pdf", bbox_inches='tight' )

f3 = plt.figure(3)

plt.plot( x_round, u_sum_default, label='static scheduler, $\pi_i = w_i$', marker='s', markersize=8 )
plt.plot( x_round, u_sum_edf, label = 'EDF', marker='^', markersize=8 )
plt.plot( x_round, u_sum_wspt, label = 'WSPT', marker='v', markersize=8 )
plt.plot( x_round, u_sum_wedf, label = '$\pi_{ij} = d_{ij} / w_{i}$', marker='o', markersize=8 )
plt.plot( x_round, u_sum_wsrt, label = 'WSRT', marker='X', markersize=8 )
plt.plot( x_round, u_sum_rms, label = 'RMS', marker='D', markersize=8 )
plt.plot( x_round, u_sum_lst, label = 'LST', marker='*', markersize=8 )

plt.xlabel( 'utilization factor $U$' )
plt.ylabel( 'normalized number of late jobs $\overline {U_w}$' )
plt.legend()
plt.grid()

f3.savefig( "uwt.pdf", bbox_inches='tight' )