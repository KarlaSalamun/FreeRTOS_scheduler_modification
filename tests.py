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

x_round = [0.1 * x for x in range(6, 15+1)]

t_sum_default, u_sum_default = compute_mean_data( x_round, default_tuples )
t_sum_edf, u_sum_edf = compute_mean_data( x_round, edf_tuples )
t_sum_wspt, u_sum_wspt = compute_mean_data( x_round, wspt_tuples )
t_sum_wsrt, u_sum_wsrt = compute_mean_data( x_round, wsrt_tuples )
t_sum_rms, u_sum_rms = compute_mean_data( x_round, rms_tuples )
t_sum_lst, u_sum_lst = compute_mean_data( x_round, lst_tuples )

f1 = plt.figure(1)
plt.scatter( x_round, t_sum_default )
plt.scatter( x_round, t_sum_edf )
plt.scatter( x_round, t_sum_wspt )
# plt.scatter( x_round, t_sum_edf_m, label = 'WEDF' )
plt.scatter( x_round, t_sum_wsrt )
plt.scatter( x_round, t_sum_rms )
plt.scatter( x_round, t_sum_lst )

plt.plot( x_round, t_sum_default, label='$\pi_{ij} = w_{ij}$' )
plt.plot( x_round, t_sum_edf, label = 'EDF' )
plt.plot( x_round, t_sum_wspt, label = 'WSPT' )
# plt.scatter( x_round, t_sum_edf_m, label = 'WEDF' )
plt.plot( x_round, t_sum_wsrt, label = 'WSRT' )
plt.plot( x_round, t_sum_rms, label = 'RMS' )
plt.plot( x_round, t_sum_lst, label = 'LST' )

plt.xlabel( 'utilization factor' )
plt.ylabel( 'normalized weighted tardiness' )
plt.grid()
plt.legend()

f1.savefig( "twt.png", bbox_inches='tight' )

f2 = plt.figure(2)
plt.scatter( x_round, u_sum_default )
plt.scatter( x_round, u_sum_edf)
plt.scatter( x_round, u_sum_wspt )
# plt.scatter( x_round, t_sum_edf_m, label = 'WEDF' )
plt.scatter( x_round, u_sum_wsrt )
plt.scatter( x_round, u_sum_rms )
plt.scatter( x_round, u_sum_lst)

plt.plot( x_round, u_sum_default, label='$\pi_{ij} = w_{ij}$' )
plt.plot( x_round, u_sum_edf, label = 'EDF' )
plt.plot( x_round, u_sum_wspt, label = 'WSPT' )
# plt.scatter( x_round, t_sum_edf_m, label = 'WEDF' )
plt.plot( x_round, u_sum_wsrt, label = 'WSRT' )
plt.plot( x_round, u_sum_rms, label = 'RMS' )
plt.plot( x_round, u_sum_lst, label = 'LST' )

plt.xlabel( 'utilization factor' )
plt.ylabel( 'normalized number of late jobs' )
plt.legend()
plt.grid()

f2.savefig( "uwt.png", bbox_inches='tight' )
# plt.show()

# fig1, (ax1, ax2) = plt.subplots(1, 2)

# ax1.scatter( x_round[0:1], t_sum_default_1, label='default scheduler' )
# ax1.scatter( x_round[0:1], t_sum_edf_1, label = 'EDF' )
# ax1.scatter( x_round[0:1], t_sum_wspt_1, label = 'WSPT' )
# ax1.scatter( x_round[0:1], t_sum_edf_m_1, label = 'WEDF' )
# ax1.scatter( x_round[0:1], t_sum_wsrt_1, label = 'WSRT' )
# ax1.plot( x_round[0:1], t_sum_default_1, label='default scheduler'  )
# ax1.plot( x_round[0:1], t_sum_edf_1, label='EDF' )
# ax1.plot( x_round[0:1], t_sum_wspt_1, label='WSPT' )
# ax1.plot( x_round[0:1], t_sum_edf_m_1, label='WEDF' )
# ax1.plot( x_round[0:1], t_sum_wsrt_1, label='WSRT' )
# ax1.set_xlabel( 'utilization factor' )
# ax1.set_ylabel( 'normalized weighted tardiness' )
# ax2 = plt.add_subplot(122)
# ax2.scatter( x_round[2:6], u_sum_default_2, label='default scheduler' )
# ax2.scatter( x_round[2:6], u_sum_edf_2, label='EDF' )
# ax2.scatter( x_round[2:6], u_sum_wspt_2, label='WSPT' )
# ax2.scatter( x_round[2:6], u_sum_edf_m_2, label='WEDF' )
# ax2.scatter( x_round[2:6], u_sum_wsrt_2, label = 'WSRT' )
# ax2.plot( x_round[2:6], u_sum_default_2, label='default scheduler' )
# ax2.plot( x_round[2:6], u_sum_edf_2, label='EDF' )
# ax2.plot( x_round[2:6], u_sum_wspt_2, label='WSPT' )
# ax2.plot( x_round[2:6], u_sum_edf_m_2, label='WEDF' )
# ax2.plot( x_round[2:6], u_sum_wsrt_2, label='WSRT' )
# ax2.set_xlabel( 'utilization factor' )
# ax2.set_ylabel( 'normalized number of late jobs' )
# plt.legend()
# plt.show()