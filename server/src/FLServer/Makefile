# $Id: Makefile 3012 2005-09-24 09:42:55Z whj $

CXX = /usr/bin/g++
CXXFLAGS = $(ZEBRA_COMPILE_OPT) -I../base -Wall -D_USE_CMD_NAMESPACE -D_ENCDEC_MSG -D_GNU_SOURCE -D_REENTRANT `mysql_config --cflags` `xml2-config --cflags`
CXXLFLAGS = $(ZEBRA_COMPILE_OPT) -Wall -L../base -lzebra -lencdec -lpthread -lz `mysql_config --libs_r` `xml2-config --libs` -llog4cxx -L/usr/local/lib -lgd -ljpeg
LIB_ZEBRA = ../base/libzebra.a

OBJS = FLServer.o ServerTask.o ServerManager.o ServerACL.o LoginTask.o LoginManager.o GYListManager.o DBAccessClient.o DBAccessClientManager.o TimeTick.o InfoClient.o InfoClientManager.o PingTask.o
SRCS = $(OBJS:%.o=%.cpp)
DEPS = $(OBJS:%.o=.%.d)

ALL_TARGETS = FLServer

all: $(ALL_TARGETS)

include $(DEPS)
$(DEPS): .%.d: %.cpp
	@$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; rm -f $@.$$$$

$(OBJS): %.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $< -o $@

$(ALL_TARGETS): $(OBJS) $(LIB_ZEBRA)
	$(CXX) $(OBJS) -o $@ $(CXXLFLAGS)

clean:
	@rm -rf *.o $(ALL_TARGETS) core *~

