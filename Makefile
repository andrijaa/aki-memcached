################################################################################
## Aki memcached server for slack
##
## File: Makefile
################################################################################
TARGET = AkiServer

TARGET_SRCS = main.cpp \
			  Connection.cpp \
			  Protocol.cpp \
			  Utils.cpp \
			  Cache.cpp \
			  AkiServer.cpp

BOOST_DIR       = /usr/local/Cellar/boost/1.57.0

BOOSTINCLUDE := -I$(BOOST_DIR)/include
BOOST_LIB_DIR = $(BOOST_DIR)/lib

BOOST_LIB = $(BOOST_LIB_DIR)/libboost_regex.a \
	$(BOOST_LIB_DIR)/libboost_filesystem.a \
	$(BOOST_LIB_DIR)/libboost_thread-mt.a \
	$(BOOST_LIB_DIR)/libboost_system.a

all: $(TARGET) 

LDLIBS =   -lpthread \
            -ldl \
            -lresolv \
			-lssl \
			-lcrypto

LDLIBS += $(BOOST_LIB)

LDFLAGS =   $(STATICFLAG) \
            -Wl,-rpath,$(BIN_DIR) \
	    	-Wl,--no-undefined

CXXFLAGS =  -Wall -g \
            -I. \
            -I.. \
	    	$(BOOSTINCLUDE)

# Disable warnings
# Disable "...will be re-ordered to match declaration order" warning
CXXFLAGS += -Wno-reorder \
            -Wno-unused-function \
            -Wno-deprecated \
            -Wno-unused-variable \
			-m64

SRCS += $(TARGET_SRCS)
OBJS := $(SRCS:.cpp=.o)

TARGET_OBJS = $(TARGET_SRCS:.cpp=.o)

# This is only in debug mode, in release libigmaster has all of the libraries
LIBS =  $(LDLIBS) $(TARGET_LDLIBS)

# Use shared libraries for debug build
$(TARGET) :  $(BIN_DIR) $(OBJS) 
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@  $(LDFLAGS) $(LIBS)

$(BIN_DIR)/$(TARGET) : $(BIN_DIR) $(TARGET)
	cp $(TARGET) $(BIN_DIR)

$(BIN_DIR) :
	mkdir -p $(BIN_DIR)

.PHONY: clean install 

install: $(BIN_DIR)/$(TARGET)

clean: 
	-rm -f $(OBJS) $(BIN_DIR)/$(TARGET) $(TARGET) >& /dev/null
