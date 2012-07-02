# This file is part of Mongoose project, http://code.google.com/p/mongoose
# This file was modified to suit HTTPd for IV:MP Servers and cannot be used as original anymore
# $Id: Makefile -1 2010-08-30 23:31:42Z Flávio Toribio $

# Possible COPT values: (in brackets are rough numbers for 'gcc -O2' on i386)
# -DHAVE_MD5		- use system md5 library (-2kb)
# -DNDEBUG		- strip off all debug code (-5kb)
# -DDEBUG		- build debug version (very noisy) (+7kb)
# -DNO_CGI		- disable CGI support (-5kb)
# -DNO_SSL		- disable SSL functionality (-2kb)
# -DCONFIG_FILE=\"file\" - use `file' as the default config file
# -DNO_SSI		- disable SSI support (-4kb)
# -DHAVE_STRTOUI64	- use system strtoui64() function for strtoull()

##########################################################################
###                 UNIX build: linux
##########################################################################

COPT=-DNDEBUG
CFLAGS=-O2 -W -Wall -std=c99 -pedantic -fomit-frame-pointer $(COPT)
LINFLAGS=-D_POSIX_SOURCE -D_BSD_SOURCE -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -lpthread $(CFLAGS)

all:
	gcc $(LINFLAGS) -c mongoose.c -o mongoose.o
	ar rcs ./lib/libmongoose.a *.o
	rm *.o
	@echo "Done.\n"
