import matplotlib.pyplot as plt

x = [1,2,4,8]
y = [1,1.404,1.498,1.567]

lx = [0,8]
ly = [0,8]

plt.plot(x,y,label='JavaThread')
plt.plot(lx,ly,label='Linear')
plt.legend()
plt.ylabel('Speedup')
plt.xlabel('Threads')
plt.show()