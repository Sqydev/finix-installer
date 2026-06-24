TARGET := finix-installer

SRC_DIR := src
OBJ_DIR := obj
BIN_DIR := compiled
LIB_DIR := libs

OUT := $(BIN_DIR)/$(TARGET)

ISO := build/base.iso
ISO_URL := https://channels.nixos.org/nixos-26.05/latest-nixos-graphical-x86_64-linux.iso

DISK := build/vm.qcow2
RAM := 4096
CPUS := 4
DISK_SIZE := 30G

CC := gcc

CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-Werror \
	-D_POSIX_C_SOURCE=200809L \
	-O2 \
	-I$(LIB_DIR)/include

LDFLAGS := -L$(LIB_DIR) -lm
LIBS := $(wildcard $(LIB_DIR)/*.a)

SRC := $(shell find $(SRC_DIR) -name "*.c")
OBJ := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))

MAKEFLAGS += --no-builtin-rules
.SUFFIXES:

.PHONY: build vm clean reset disk test

build: $(OUT)

$(OUT): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) $(LIBS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

$(ISO):
	@mkdir -p build
	curl -L $(ISO_URL) -o $(ISO)

disk:
	@mkdir -p build
	@if [ ! -f "$(DISK)" ]; then \
		qemu-img create -f qcow2 $(DISK) $(DISK_SIZE); \
	fi

vm: $(ISO) disk
	qemu-system-x86_64 \
		-enable-kvm \
		-m $(RAM) \
		-smp $(CPUS) \
		-cdrom $(ISO) \
		-drive file=$(DISK),format=qcow2 \
		-virtfs local,path=$(BIN_DIR),mount_tag=INSTALLER,security_model=mapped,id=INSTALLER \
		-boot order=d

test: build vm

reset:
	rm -f $(DISK)
	$(MAKE) disk

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR) build
