import matplotlib.pyplot as plt
import numpy as np


np.random.seed(0)

x, y   = np.random.randn(2, 200)
fig = plt.figure() # definir la figure 

#ajour du premier subplot avec 3 ligne et 1 colonne 
ax1 = fig.add_subplot(311)  
ax1.xcorr(x, y, usevlines=True, maxlags=50, normed=True, lw=2)
#ax1.grid(True) // cette fontion sert a afficher la grille 
ax1.axhline(0, color='red', lw=2)

#ajour du duxième subplot avec 2 ligne et 1 colonne  
ax2 = fig.add_subplot(312, sharex=ax1)
ax2.acorr(x, usevlines=True, normed=True, maxlags=50, lw=2)
#ax2.grid(True)
ax2.axhline(0, color='green', lw=2)

ax3 = fig.add_subplot(313)
ax3.xcorr(x,y, usevlines=True, normed=True, maxlags=50, lw=2)
#ax3.grid(True)
ax3.axhline(0, color='black', lw=2) 

plt.show()

