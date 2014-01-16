# compiler
CC = $(CXX)

# compile-time flags
# CFLAGS = -Wall -g
CFLAGS = -g -std=c++0x
PTHREAD = -pthread

# path to compiled boost library (necessary only if it isn't in compilers include path)
BOOST_PATH = ../boost
BOOST_HEADERS = $(BOOST_PATH)/include
BOOST_LIBS = $(BOOST_PATH)/lib

BOOST_INCLUDE = -I $(BOOST_HEADERS) -L $(BOOST_LIBS)

BOOST_SYSTEM = $(BOOST_LIBS)/libboost_system.a
BOOST_SERIALIZATION = $(BOOST_LIBS)/libboost_serialization.a



# match all *.cc files and make *.o files
OBJS = $(patsubst %.cc, %.o, $(wildcard *.cc))

# program name
TARGET = cserver tclient


$(OBJS): %.o: %.cc
	$(CC) -c $< $(CFLAGS) $(BOOST_INCLUDE) -o $@

cserver: $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(BOOST_INCLUDE) -o $@

tclient: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(BOOST_INCLUDE) $(BOOST_SYSTEM) $(PTHREAD) -o $@

clean:
	$(RM) $(OBJS) $(TARGET)