# For CC2650 use the following target and one of the two boards
# Don't forget to make clean when you are changing the board
#TARGET ?= srf06-cc26xx
#BOARD  ?= sensortag/cc2650
#BOARD  ?= launchpad/cc2650

# For TMote Sky (emulated in Cooja) use the following target
TARGET ?= sky

DEFINES=PROJECT_CONF_H=\"project-conf.h\"
CONTIKI_PROJECT = app

PROJECT_SOURCEFILES += my_collect.c
PROJECT_SOURCEFILES += my_routing_table.c

all: $(CONTIKI_PROJECT)

CONTIKI_WITH_RIME = 1
CONTIKI ?= ../../contiki
include $(CONTIKI)/Makefile.include
