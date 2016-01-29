f=open('parallel2.txt','r')
f1=open('serial.txt','r')
f2=open('speedup02.txt','w')
for i in xrange(0,9):
	a=f.readline().strip('\n')
	b=f1.readline().strip('\n')
	temp=float(b)/float(a)
	f2.write(str(temp)+str("\n"))
