#=======================================================================
#  Makefile for the program `makeindex'
#
#  Copyright (C) 1987	Pehong Chen	(phc@renoir.berkeley.edu)
#  Computer Science Division
#  University of California, Berkeley
#
# On many UNIX systems, do
#	make
# to build MakeIndex, and
#	make install
# to install it.  You may need to modify BINDIR, MANDIR, and MANEXT
# for your system.
#
# For systems that require different compile flags, there are
# additional targets apollo .. vms that can be used, e.g.
#	make hp720
#
# Current target list:
#	all
#	$(BINDIR)/$(PROGRAM)
#	distclean
#	depend
#	dist
#	index
#	install
#	man
#	mostlyclean
#	print
#	program
#	$(PROGRAM)
#	realclean
#	reallyclean
#	shar
#	tar
#	tags
#	TAGS
#	test
#	uninstall
#	update
#
#	apollo
#	du42
#	du43
#	du42-gcc
#	du43-gcc
#	du42-lcc
#	du43-lcc
#	hp375
#	hp720
#	hp850
#	hp850-c89
#	ibm370
#	ibm370-c++
#	ibmpc
#	ibmps2
#	ibmrs6000
#	ibmrs6000-c++
#	mips
#	next
#	next-g++
#	next-objc
#	sgi
#	sgi-c++
#	sgi-g++
#	solaris
#	solaris-apcc
#	solaris-c++
#	solaris-g++
#	stardent
#	sun
#	sun-apcc
#	sun-c++
#	sun-g++
#	vms
#
# [26-May-1993] -- update for 2.12beta release (add GNU Project
# 		   standard targets, alphabetize macros and targets,
# 		   replace use of install command)
# [20-Apr-1993] -- remove references to system include files
# [07-Aug-1991]
#=======================================================================

ALL		= $(MAKEFILE) $(DOC) $(MANUAL) $(HDRS) $(SRCS)

BINDIR		= /usr/local/bin

CFLAGS		= -O

CHMOD		= chmod

CP		= /bin/cp

DOC		= makeindex.tex

#EXTHDRS	= /usr/include/ctype.h \
#		/usr/include/stdio.h

HDRS		= genind.h \
		mkind.h \
		scanid.h \
		scanst.h

LDFLAGS		= $(CFLAGS)

LIBS		=

LINKER		= $(CC)

MAKEFILE	= Makefile

MANDIR		= /usr/local/man/man1

MANEXT		= 1

MANUAL		= makeindex.l

OBJS		= genind.o \
		mkind.o \
		qsort.o \
		scanid.o \
		scanst.o \
		sortid.o

PRINT		= psgrind

PROGRAM		= makeindex

RM		= /bin/rm -f

SHAR		= $(PROGRAM).shar

SHELL		= /bin/sh

SRCS		= genind.c \
		mkind.c \
		qsort.c \
		scanid.c \
		scanst.c \
		sortid.c

STRIP		= strip

TAR		= $(PROGRAM).tar

#=======================================================================

all:		$(PROGRAM)

$(BINDIR)/$(PROGRAM): $(SRCS) $(HDRS) $(EXTHDRS)
	@$(MAKE) -f $(MAKEFILE) BINDIR=$(BINDIR) install

clean mostlyclean:
	-$(RM) $(OBJS)
	-$(RM) $(PROGRAM)
	-$(RM) *.out
	-$(RM) *~
	-$(RM) \#*
	-$(RM) core

clobber realclean reallyclean:	clean

distclean:	clobber
	-$(RM) $(SHAR) $(TAR) tags TAGS

depend:
	-@$(RM) -f .#*.[chly]
	mkmf -f $(MAKEFILE) PROGRAM=$(PROGRAM) BINDIR=$(BINDIR)

dist:
	$(CP) $(PROGRAM) $(BINDIR)
	rcp $(PROGRAM) monet:$(BINDIR)
	rcp $(PROGRAM) arpa:$(BINDIR)
	rcp $(PROGRAM) harrison@vangogh:bin

index:
	@ctags -wx $(HDRS) $(SRCS)

install:	$(PROGRAM)
	$(CP) $(PROGRAM) $(BINDIR)/$(PROGRAM)
	$(STRIP) $(BINDIR)/$(PROGRAM)
	$(CHMOD) 0755 $(BINDIR)/$(PROGRAM)
	$(CP) ../doc/$(MANUAL) $(MANDIR)/makeindex.$(MANEXT)
	$(CHMOD) 0744 $(MANDIR)/makeindex.$(MANEXT)
	@ls -lgs $(BINDIR)/$(PROGRAM) $(MANDIR)/makeindex.$(MANEXT)

man:
	ptroff -man $(MANUAL)

print:
	@$(PRINT) $(HDRS) $(SRCS)

program:        $(PROGRAM)

$(PROGRAM):     $(OBJS)
	-@$(RM) -f $(PROGRAM)
	$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
	@size $(PROGRAM)

shar:
	-@$(RM) -f $(SHAR)
	shar $(SHAR) $(ALL)

tar:
	-@$(RM) $(TAR)
	tar -cf $(TAR) $(ALL)

tags:	$(HDRS) $(SRCS)
	@ctags $(HDRS) $(SRCS)

TAGS:	$(HDRS) $(SRCS)
	@etags $(HDRS) $(SRCS)	

test:	$(PROGRAM)
	cd ../test ; $(MAKE)

uninstall:
	-$(RM) $(BINDIR)/$(PROGRAM)
	-$(RM) $(MANDIR)/makeindex.$(MANEXT)

update:		$(BINDIR)/$(PROGRAM)

.DEFAULT:
	co $@

#=======================================================================
# Machine-specific targets to simplify installation when additional
# compile flags are needed.
#
# Apollo DN/10000
apollo:
	$(MAKE) CFLAGS='-DOS_SYSV -U__STDC__' $(PROGRAM)

# DECstation Ultrix 4.2 and 4.3
du42 du43:	all

# DECstation Ultrix 4.2 and 4.3 with gcc
du42-gcc du43-gcc:
	$(MAKE) CC=gcc $(PROGRAM)

# DECstation Ultrix 4.2 and 4.3 with lcc
du42-lcc du43-lcc:
	$(MAKE) CC=lcc CFLAGS= $(PROGRAM)

# HP 375 (4.3BSD)
hp375:	all

# HP 9000/720 (HP-UX, System V UNIX)
hp720:
	$(MAKE) CFLAGS='-DOS_SYSV' $(PROGRAM)

# HP 9000/850S (HP-UX, System V UNIX)
hp850:
	$(MAKE) CFLAGS='-DOS_SYSV' $(PROGRAM)

# HP 9000/850S (HP-UX, System V UNIX) with Standard C compiler
hp850-c89:
	$(MAKE) CC=c89 CFLAGS='-DOS_SYSV' $(PROGRAM)

# IBM 3090/600S-VF AIX
ibm370:
	$(MAKE) CFLAGS='-D_BSD' $(PROGRAM)

# IBM 3090/600S-VF AIX with C++
ibm370-c++:
	$(MAKE) CC-'CC +a1 +p +w -D_POSIX_SOURCE -Hansi -D__STDC__ \
		-D_BSD -D_AIX -D_AIX370' CFLAGS='-D_BSD' $(PROGRAM)

# IBM PC DOS
ibmpc:	all

# IBM PS/2 AIX 2.1
ibmps2:	all

# IBM PS/2 AIX 2.1 with C++
ibmps2-c++:
	$(MAKE) CC='CC +a1 +p +w -D_POSIX_SOURCE -Hansi -D__STDC__ \
		-D_BSD -D_AIX -Dps2' CFLAGS='-D_BSD' $(PROGRAM)

# IBM RS/6000 AIX 3.1
ibmrs6000:
	$(MAKE) $(PROGRAM)

# IBM RS/6000 AIX 3.1 with C++
ibmrs6000-c++:
	$(MAKE) CC=xlC $(PROGRAM)

# MIPS R6000 RISC/os 5.0
mips:
	$(MAKE) CFLAGS='-DOS_SYSV -O' $(PROGRAM)

# NeXT Mach
next:	all

# NeXT Mach with C++
next-g++:
	$(MAKE) CC=g++ $(PROGRAM)

# NeXT Mach with Objective C
next-objc:
	$(MAKE) CC='cc -ObjC' $(PROGRAM)

# Silicon Graphics IRIX 3.x and 4.x
sgi:
	$(MAKE) CFLAGS='-DOS_SYSV -O2' $(PROGRAM)

sgi-c++:
	$(MAKE) CC=CC CFLAGS='-DOS_SYSV -O2' $(PROGRAM)

sgi-g++:
	$(MAKE) CC=g++ CFLAGS='-DOS_SYSV -O2' $(PROGRAM)

# Sun Solaris 2.x (AT&T SysVR4): must compile in ANSI environment
solaris:
	$(MAKE) CFLAGS='-Xc -xO4' all

# Sun Solaris 2.x (AT&T SysVR4) with Apogee C
solaris-apcc:
	$(MAKE) CC='apcc -Xc=ansi' CFLAGS='-O4' all

# Sun Solaris 2.x (AT&T SysVR4) with C++
solaris-c++:
	$(MAKE) CC=CC CFLAGS='-O4' all

# Sun Solaris 2.x (AT&T SysVR4) with C++
solaris-g++:
	$(MAKE) CC=g++ all

# Stardent 1520 UNIX Release 2.2
stardent:	all

# SunOS 4.1.x (BSD UNIX)
sun:	all

# SunOS 4.1.x (BSD UNIX) with Apogee C
sun-apcc:
	$(MAKE) CC=apcc all

# SunOS 4.1.x (BSD UNIX) with C++
sun-c++:
	$(MAKE) CC=CC all

# SunOS 4.1.x (BSD UNIX) with g++
sun-g++:
	$(MAKE) CC=g++ all

# VAX VMS 5.3
vms:	all

#=======================================================================
###
genind.o: mkind.h genind.h
mkind.o: mkind.h
qsort.o: mkind.h
scanid.o: mkind.h scanid.h
scanst.o: mkind.h scanst.h
sortid.o: mkind.h
