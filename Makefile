CC=gcc
CCFLAGS=-c -Wall -Wextra -Isrc -g

LD=gcc
LDFLAGS=-lX11 -g

DEL=@rm -frdv
MKDIR=@mkdir -p
ECHO=@echo

XINIT=xinit
XEPHYR:=$(shell (whereis -b Xephyr | cut -f2 -d ' '))
XDISPLAY=:100
XEPHYRFLAGS=-sw-cursor -retro -ac -screen 800x600 -host-cursor

SRCS=$(wildcard src/*.c)

DEPS=$(patsubst src/%,deps/%.d,$(SRCS))
OBJS=$(patsubst src/%,bin/%.o,$(SRCS))

.PHONY: all clear debug release run

all: build


profile: build
	$(XINIT) $(SHELL) profile.xinitrc -- \
		"$(XEPHYR)" $(XDISPLAY) $(XEPHYRFLAGS)

debug: build
	$(XINIT) $(SHELL) debug.xinitrc -- \
		"$(XEPHYR)" $(XDISPLAY) $(XEPHYRFLAGS)

run: build
	$(XINIT) $(SHELL) run.xinitrc -- \
		"$(XEPHYR)" $(XDISPLAY) $(XEPHYRFLAGS)


clean:
	$(DEL) bin
	$(DEL) deps
	$(DEL) build

build: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

-include $(DEPS)

deps/%.c.d: src/%.c
	$(MKDIR) $(dir $@)
	$(CC) $(CCFLAGS) -M -o $@ $< -MT $(patsubst deps/%.d,bin/%.o,$@)

bin/%.c.o: src/%.c
	$(MKDIR) $(dir $@) 
	$(CC) $(CCFLAGS) -o $@ $<

