#!/usr/bin/env python
# coding: utf-8

# Leire Larizgoitia Arcocha

from __future__ import division

import matplotlib
matplotlib.rcParams['text.usetex'] = True
from matplotlib import pyplot as plt
#plt.rcParams.update({'font.size': 20})
import matplotlib.colors as mcolors
import matplotlib.ticker as mtick
from matplotlib.ticker import MultipleLocator
import numpy as np
import math

import uproot
#file = uproot.open("detHe3_ntuple.root")
file = uproot.open("OSURRHe3_1e6nthermalR1-16mm.root")

'NTUPLE READ'

keys =file.keys()
events = file['Target']
ev_col = events.keys()
ev_branches = events.arrays()

print(ev_col)
print()

events_ = ev_branches['Event']
track_ = ev_branches['Track']
parent_ = ev_branches['Parent']
particleName_ = ev_branches['ParticleName']
energy_ = ev_branches['Energy']
inittime_ = ev_branches['InitTime']
finaltime_ = ev_branches['FinalTime']
initVol_ = ev_branches['InitVolume']
process_ = ev_branches['CreatorProcess']


#print(particleName_.tolist())

totParticlesProduced = len(events_)
print("Total Particles produced", totParticlesProduced)

# using set()
# to remove duplicated
# from list
events_ = list(set(events_))

# printing list after removal
# distorted ordering
#print("The list after removing duplicates : " ,  str(events_))

reactionProduced = len(events_)
print("Total Daughter Particles produced", reactionProduced)

print()

''' 1e6 NEUTRONS '''

neutronsGenerated = 1e6

'Total Particles produced 2049'

#reactionProduced =  597

print('Neutrons run/beamOn ', neutronsGenerated)
print("Total Daughter Particles produced ", reactionProduced)

efficiency = reactionProduced / neutronsGenerated
print('OSURR He3 detector efficiency ', efficiency)

#sensitivity_iso = efficiency /0.0387
#print('OSURR He3 detector sensitivity ', sensitivity_iso)

print()

inch = 2.54 #cm
radBeam_ = 0.9375*inch/2 #cm
sBeam_ = np.pi * radBeam_**2

radColim_ = 0.116/2 #cm
sColim_ = np.pi * radColim_**2
print(sColim_)
print(sBeam_)
sCross_ = sColim_ / sBeam_

print("Surface Ratio" , 1/sCross_)
print('Normalized He3  detector efficiency ', 1/sCross_ * efficiency)

'OSURR Normalized He3 detector efficiency 25.16%'


eff_Diam = 243.8 #cm
eff_Length = 2032.
sDet = eff_Length * eff_Diam

cps_NV = efficiency * sDet

print(cps_NV)

"Normalized He3 detector efficiency with isotropic flux:  0.25935"


# histogram of the data
#n, bins, patches = plt.hist(energy_, 10, density=True, facecolor='g') #len(energy_)

#plt.xlabel('Energy (eV)')
#plt.ylabel('Counts/bin')
#plt.grid(True)
#plt.show()

"END OF CODE"
