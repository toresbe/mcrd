CC=cc
CXX=g++
RM=rm -f
CPPFLAGS=-g -fPIC -std=gnu++17 -Wall -Wextra -I. -DBOOST_LOG_DYN_LINK
LDFLAGS=-g 
LDLIBS=-lpthread -lboost_log -lboost_program_options

PROG=mcrd
SRCS=$(wildcard *.cpp)
OBJS=$(subst .cpp,.o,$(SRCS)) 

all:		$(PROG)


$(PROG):	$(OBJS)
		$(CXX) $(LDFLAGS) -o $(PROG) $(OBJS) $(LDLIBS) 

clean:
	    $(RM) $(OBJS)

distclean: clean
	    $(RM) mcrd
