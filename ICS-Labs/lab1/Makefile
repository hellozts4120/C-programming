# Student's Makefile for 15-213 Lab 1
TEAM = ac00
VERSION = 1
HANDINDIR = /afs/cs.cmu.edu/academic/class/15213-s03/labs/L1/handin

CC = gcc
CFLAGS = -O -Wall

btest: btest.c bits.c decl.c tests.c btest.h bits.h
	$(CC) $(CFLAGS) -o btest bits.c btest.c decl.c tests.c

handin:
ifeq ($(TEAM),ac00)
	echo -e "\n\nERROR:\n\nYou must specify TEAM in order to hand-in your code.\nNo hand-in was made.\n\n"
else
	cp bits.c $(HANDINDIR)/$(TEAM)-$(VERSION)-bits.c
endif
clean:
	rm -f *.o btest

