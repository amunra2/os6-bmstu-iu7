CURRENT = $(shell uname -r)
KDIR = /lib/modules/$(CURRENT)/build
PWD = $(shell pwd)
TARGET1 = md
obj-m := $(TARGET1).o


// Собрать модуль ядра
default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules


// Очистить промежуточные файлы, полученные при сборке
clean:
	@rm -f *.o .*.cmd .*.flags *.mod.c *.order Module.*
	@rm -f .*.*.cmd *~ *.*~ TODO.*
	@rm -fR .tmp*
	@rm -rf .tmp_versions


// Очистить скомпилированный файл модуля ядра
disclean: clean
	@rm *.ko *.symvers *.mod


// Загрузить модуль ядра
load:
	sudo insmod md.ko


// Выгрузить модуль ядра
remove:
	sudo rmmod md.ko


// Информация о загруженном модуле ядра
info:
	modinfo md.ko


// Вывод логов
logs:
	dmesg