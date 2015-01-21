BASEDIR:=$(CURDIR)
SRCDIR:=$(BASEDIR)/src
PREFIXDIR:=$(CURDIR)/install

# If these are defined in your environment it will override them
# Put environment overrides in this file
MAKE_ENVIRONMENT?=$(BASEDIR)/environment.mk # Overrides environment variables
MAKE_TGTS?=$(BASEDIR)/targets.mk # Adds custom make targets
MAKE_SOURCES?=$(BASEDIR)/sources.mk

vpath %.c $(SRCDIR)
vpath %.h $(SRCDIR) $(BASEDIR)/arduino

include $(MAKE_SOURCES)

# Begin environment overrides
AVR_ROOT:=/usr

CC:=$(AVR_ROOT)/bin/avr-gcc
LD:=$(AVR_ROOT)/bin/avr-gcc
OBJ_COPY:=$(AVR_ROOT)/bin/avr-objcopy
SIZE:=$(AVR_ROOT)/bin/avr-size
TERM:=/usr/bin/ckermit
INST:=/usr/bin/avrdude

ARCH:=atmega328p
BUILD_TYPE:=release

TARGET:=main
TARGET_ELF:=$(TARGET).elf
TARGET_HEX:=$(TARGET).hex
TARGET_MAP:=$(TARGET).map
UART_BAUD:=38400
F_CPU:=16000000L

INST_DEV:=/dev/ttyUSB0
INST_PROTO:=arduino
INST_BAUD:=57600
INST_ARCH:=m328p
INST_FLAGS=-c $(INST_PROTO) -p $(INST_ARCH) -P $(INST_DEV) -b $(INST_BAUD)
INST_FLASH=-U flash:w:$(TARGET_HEX):i

TERM_FLAGS:=-C "set line $(INST_DEV),set speed $(UART_BAUD),set carrier-watch off,connect"

# End environment overrides
CPPFLAGS:=
CFLAGS:=
LDFLAGS:=-Wl,-Map,$(TARGET_MAP) -mmcu=$(ARCH)

ifneq ($(strip $(wildcard $(MAKE_ENVIRONMENT))),)
  include $(MAKE_ENVIRONMENT)
endif

CPPFLAGS+=-DF_CPU=$(F_CPU)
CFLAGS+=-mmcu=$(ARCH) -Wall -Werror --std=c99

OBJS:=$(SRCS:%.c=%.o)
DEP_FILES:=$(OBJS:%.o=%.d)

ifeq ($(BUILD_TYPE),release)
  CFLAGS+=-Os
else
  CFLAGS+=-g
endif

.PHONY: all
all: $(TARGET_HEX)

$(TARGET_ELF): $(OBJS)
	$(LD) $(LDFLAGS) -o $(TARGET_ELF) $(OBJS)
	$(SIZE) $(TARGET_ELF)

$(TARGET_HEX): $(TARGET_ELF)
	$(OBJ_COPY) -O ihex $(TARGET_ELF) $(TARGET_HEX)

ifneq ($(MAKECMDGOALS),clean)
 include $(DEP_FILES)
endif

%.d: %.c
	$(CC) $(CPPFLAGS) -M -MG $< | sed 's|\($*.o\):|\1 $@:|g' > $@

.PHONY: debug
debug:
	@$(MAKE) --no-print-dir BUILD_TYPE=debug

.PHONY: release
release:
	@$(MAKE) --no-print-dir BUILD_TYPE=release

.PHONY: upload
upload: $(TARGET_HEX)
	$(INST) $(INST_FLAGS) $(INST_FLASH)

.PHONY: clean
clean:
	rm -rf *.o *.d $(TARGET_ELF) $(TARGET_HEX) $(TARGET_MAP)

.PHONY: kermit
term:
	$(TERM) $(TERM_FLAGS)

$(PREFIXDIR)/lib:
	mkdir -p $@

ifneq ($(strip $(wildcard $(MAKE_TGTS))),)
 include $(MAKE_TGTS)
endif
