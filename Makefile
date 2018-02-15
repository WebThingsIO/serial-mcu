# Turn on increased build verbosity by defining BUILD_VERBOSE in your main
# Makefile or in your environment. You can also use V=1 on the make command
# line.

ifeq ("$(origin V)", "command line")
BUILD_VERBOSE=$(V)
endif
ifndef BUILD_VERBOSE
BUILD_VERBOSE = 0
endif
ifeq ($(BUILD_VERBOSE),0)
Q = @
else
Q =
endif
# Since this is a new feature, advertise it
ifeq ($(BUILD_VERBOSE),0)
$(info Use make V=1 or set BUILD_VERBOSE in your environment to increase build verbosity.)
endif

RM = rm
ECHO = @echo

CROSS_COMPILE =

AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
CXX = $(CROSS_COMPILE)g++
LD = $(CROSS_COMPILE)ld
GDB = $(CROSS_COMPILE)gdb
OBJCOPY = $(CROSS_COMPILE)objcopy
SIZE = $(CROSS_COMPILE)size

BUILD = build

INC =  -I.
INC += -I../ArduinoJson

CXXFLAGS = $(INC) -Wall $(COPT)
LDFLAGS =

#Debugging/Optimization
ifeq ($(DEBUG), 1)
CXXFLAGS += -g
LDFLAGS += -g
COPT = -O0
BUILD = build-debug
else
COPT += -Os -DNDEBUG
endif

COMMON_OBJ = $(addprefix $(BUILD)/,\
  ClientSocketPort.o \
  ServerSocketPort.o \
	Packet.o \
	SocketPort.o \
	)

TARGETS = example json-test echo-server echo-client

all: $(TARGETS)

echo-client: $(COMMON_OBJ)
echo-server: $(COMMON_OBJ)
example: $(COMMON_OBJ)

define compile_cxx
$(ECHO) "CXX $<"
$(Q)$(CXX) $(CXXFLAGS) -c -MD -o $@ $<
@# The following fixes the dependency file.
@# See http://make.paulandlesley.org/autodep.html for details.
@cp $(@:.o=.d) $(@:.o=.P); \
  sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
      -e '/^$$/ d' -e 's/$$/ :/' < $(@:.o=.d) >> $(@:.o=.P); \
  rm -f $(@:.o=.d)
endef

$(BUILD)/%.o: %.cpp | $(BUILD)
	$(call compile_cxx)

$(OBJ): | $(BUILD)
$(BUILD):
	mkdir -p $@

$(TARGETS): %: $(BUILD)/%.o
	$(ECHO) "Linking $@"
	$(Q)$(CXX) $(LDFLAGS) -o $@ $^

clean:
	$(RM) -rf $(BUILD) $(TARGETS)
.PHONY: clean

-include $(wildcard $(BUILD)/*.P)
