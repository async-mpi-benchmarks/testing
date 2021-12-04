import numpy as np
import matplotlib.pyplot as plt

x = np.arange(0, 100, 1) #start=0 , stop=100 , step = 5
y = np.cos(x)
plt.plot(x, y)
plt.show() 