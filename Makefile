all: cfs_sched test_multimap

cfs_sched: cfs_sched.cc multimap.h
	g++ -g -Wall -Werror -o $@ $< -std=c++11

test_multimap: test_multimap.cc multimap.h
	g++ -Wall -Werror -o $@ $< -std=c++11 -pthread -lgtest

clean: 
	-rm -f cfs_sched. test_multiset