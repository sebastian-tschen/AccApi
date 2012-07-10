#####generic conf
SRC_FILTER = .c
LIBS  = 
LIBS += 
CFLAGS = -I.
LDFLAGS =
CONF = .
SOURCES :=  $(shell find ./ -type f -name '*${SRC_FILTER}')
SONAME = libaccapi.so.1.0.0
LDSOFLAGS = -shared -Wl,-soname,libaccapi.so.1
################

################target_specific conf

##### single target without a name - so we just call "make" without CONF=... for this target
ifeq ($(CONF),.)
#mkres := $(shell mkdir $(CONF))
CFLAGS += -g -fPIC
TESTBINDIR = bin
TESTBIN = $(TESTBINDIR)/testsensor
CC = gcc
LD = gcc
endif
####

################


OBJS := $(patsubst ./%${SRC_FILTER}, ${CONF}/%.o, $(SOURCES))

#all: post-build
#post-build: $(TESTBIN)
#   @echo post_build_step if you want to run somthing here like copy the binary to somewhere, etc.
#

all: $(TESTBIN) $(SONAME)

$(TESTBINDIR):
	mkdir -p $@

$(TESTBIN): ${OBJS} ${TESTBINDIR}
	$(LD) ${OBJS} ${LIBS} ${LDFLAGS} -o ${CONF}/"$(TESTBIN)"

${CONF}/%.o: %${SRC_FILTER}
	$(CC) $(CFLAGS) -o $@ -c $<

$(SONAME): ${OBJS}
	$(LD) $(LDSOFLAGS) -o $(SONAME) ${OBJS}

so: $(SONAME)

clean:
	rm -f ${CONF}/$(TESTBIN)
	rm -f $(OBJS)
	rm -rf $(TESTBINDIR)
	rm -f $(SONAME)



