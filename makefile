ISO := build/base.iso
ISO_URL := https://channels.nixos.org/nixos-26.05/latest-nixos-graphical-x86_64-linux.iso

DISK := build/vm.qcow2
RAM := 4096
CPUS := 4
DISK_SIZE := 30G

CAL_CFG := ./src/calamares
CFG_DISK := build/calamares.img

.PHONY: run disk install clean reset

$(ISO):
	mkdir -p build
	@echo "Downloading ISO..."
	curl -L $(ISO_URL) -o $(ISO)

disk:
	mkdir -p build
	@if [ ! -f "$(DISK)" ]; then \
		qemu-img create -f qcow2 $(DISK) $(DISK_SIZE); \
	fi

install: $(ISO)

run: $(ISO) disk
	qemu-system-x86_64 \
		-enable-kvm \
		-m $(RAM) \
		-smp $(CPUS) \
		-cdrom $(ISO) \
		-drive file=$(DISK),format=qcow2 \
		-virtfs local,path=$(CAL_CFG),mount_tag=CALAMARES,security_model=mapped,id=CALAMARES \
		-boot order=d

reset:
	rm -f $(DISK)
	$(MAKE) disk

clean:
	rm -rf build
