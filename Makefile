#
# Makefile to make the file libclientserver.a, containing
# connection.o and server.o
#
# Define the compiler. g++ can be
# changed to clang++.
CXX = g++
CC  = g++

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of libstdc++.
CPPFLAGS =  -std=c++11
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast
CXXFLAGS += -std=c++11 
CXXFLAGS += -g -L
VPATH		 += src:sqlite_src
OUT_DIR  += bin
#CXXFLAGS =  -stdlib=libc++
#CPPFLAGS =  -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
LDLIBS=-lpthread -ldl

all: libclientserver.a sqlite memoryserver sqlserver client

install: restructure

sqlite:
	cd ./sqlite_src; gcc -c sqlite3.c; mv sqlite3.o ../
#	cd ./sqlite_src; ./configure; make;

test: libclientserver.a sqlite dbtest dbtestrestructure

# Create the library; ranlib is for Darwin (OS X) and maybe other systems.
# Doesn't seem to do any damage on other systems.

libclientserver.a: connection.o server.o
	ar rv libclientserver.a  connection.o server.o
	ranlib libclientserver.a

memoryserver: memoryserver.o newsserver.o  connection.o server.o messagehandler.o message.o inmemoryarticle.o inmemorynewsgroup.o inmemorydatabase.o
sqlserver: sqlserver.o newsserver.o server.o connection.o messagehandler.o message.o sqlitedatabase.o sqlitenewsgroup.o sqlitearticle.o sqlite3.o

client: connection.o servercommunication.o messagehandler.o message.o remotedatabase.o remotenewsgroup.o remotearticle.o

# dbtest: inmemoryarticle.o inmemorynewsgroup.o inmemorydatabase.o sqlitearticle.o sqlitenewsgroup.o sqlitedatabase.o dbtest.o sqlite3.o

restructure: 
	mkdir -p bin; rm bin/*; mv *.o bin/; mv *.a bin/; mv client bin/; mv memoryserver bin/; mv sqlserver bin/;

dbtestrestructure:
	mkdir -p bin; rm bin/*; mv *.o bin/; mv *.a bin/; mv dbtest bin/;

# Phony targets
.PHONY: all

# Standard clean
clean:
	rm -f *.o; rm -f bin/*.o bin/libclientserver.a

# Generate dependencies in *.d files
%.d: %.cc
	@set -e; rm -f $@; \
         $(CPP) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
         rm -f $@.$$$$

# Include the *.d files
 
#SRC_DIR = src
#SRC = $(SRC_DIR)/$(wildcard *.cc)
SRC = $(wildcard *.cc)
include $(SRC:.cc=.d)
