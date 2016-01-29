import random
n=int(raw_input())
print n
print (n*(n-1))/2

edges=[]
cost=[]
for i in xrange(0,n):
	for j in xrange(i+1,n):
		edges.append((i,j))
		temp=random.randint(1,100)
		cost.append(temp)
for i in xrange(0,len(edges)):
	print str(edges[i][0])+' '+str(edges[i][1])+' '+str(cost[i])
print 0