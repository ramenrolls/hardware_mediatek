#!/usr/bin/env python2.6

#
# Generates a table of prime numbers for use in BasicHashtable.cpp.
#
# Each prime is chosen such that it is a little more than twice as large as
# the previous prime in the table.  This makes it easier to choose a new
# hashtable size when the underlying array is grown by as nominal factor
# of two each time.
#

def is_odd_prime(n):
  limit = (n - 1) / 2
  d = 3
  while d <= limit:
    if n % d == 0:
      return False
    d += 2
  return True

print "static size_t PRIMES[] = {"

n = 5
max = 2**31 - 1
while n < max:
  print "    %d," % (n)
  n = n * 2 + 1
  while not is_odd_prime(n):
    n += 2

print "    0,"
print "};"
