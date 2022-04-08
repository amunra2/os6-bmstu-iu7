CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
TARGET = fortune# fortune OR seqfile
obj-m := $(TARGET).o

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order Module.*
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions
	@rm *.ko *.mod

disclean: clean
	@rm *.ko *.symvers *.mod

load:
	sudo insmod ${TARGET}.ko

remove:
	sudo rmmod ${TARGET}.ko

info:
	modinfo ${TARGET}.ko

logs:
	dmesg

update:
	make clean && make remove && make && make load


test:
	@echo "aaa" > /proc/fortune_file
	@echo "bbb" > /proc/fortune_file
	@echo "ccc" > /proc/fortune_file
	@echo "ddd" > /proc/fortune_file
	@echo "eee" > /proc/fortune_file
	@cat /proc/fortune_file
	@cat /proc/fortune_file
	@cat /proc/fortune_file
	@cat /proc/fortune_file
	@cat /proc/fortune_file
