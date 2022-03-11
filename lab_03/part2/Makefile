obj-m += md1.o md2.o md3.o

KERNELDIR ?= /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install help clean:
	$(MAKE) -C $(KERNELDIR) M=$(shell pwd) $@


# Очистить промежуточные файлы, полученные при сборке
clean: 
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order
	@rm -f .*.*.cmd *~ *.*~ TODO.* 
	@rm -fR .tmp* 
	@rm -rf .tmp_versions 


# Очистить скомпилированный файл модуля ядра
disclean: clean 
	@rm *.ko *.symvers *.mod


# Загрузить модули ядра
load:
	sudo insmod md1.ko
	sudo insmod md2.ko
	sudo insmod md3.ko


# Выгрузить модули ядра
remove:
	sudo rmmod md2.ko
	sudo rmmod md1.ko


# Показать то, что модули ядра загружены
md_list:
	lsmod | grep md


# Вывод логов
logs:
	dmesg | grep +


# Информация о загруженном модуле ядра
info:
	modinfo md1.ko
	modinfo md2.ko


