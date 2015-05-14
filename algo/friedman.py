#!/usr/bin/env python2
# coding: utf-8

import sys
import time
from itertools import*
from threading import Thread



def friedman(nb) :
    res = ''
    s=lambda x:[x]['1'>x>'0':]+['(%s%s%s)'%f for i in range(1,len(x))for f in product(s(x[:i]),'*/-+^',s(x[i:]))]
    def E(e):
        try:

            string = "a = "+e.replace("^","**")
            if(len(e.split('^',))<=2):
                code = compile(string, '<string>', 'exec')
                ns = {}
                exec code in ns
                return ns['a']
        except:
            0
    A={i:e for i in range(nb,nb+1)for x in permutations(`i`)for e in s("".join(x))[x>='1':]if E(e)==i}
    for v in A:
        res += "{}:{}".format(v,A[v])
    return res

print >> sys.stderr, 'FRIEDMANN CALCULATOR : ' + sys.argv[1]
friedman_numbers = friedman(int(sys.argv[1]))
sys.stdout.write(friedman_numbers)
print >> sys.stderr, friedman_numbers
