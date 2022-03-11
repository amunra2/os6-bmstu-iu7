#include <linux/init.h>	  // ​Макросы __init и ​__exit
#include <linux/module.h> // MODULE_*

#include "md.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_03 (Part 2): md1");


char *md1_data = "Hello, World!";


// Внешние модули могут использовать те имена,
// которые явно экспортированы

// Подходит для связывания
extern char *md1_proc(void)
{
    return md1_data;
}


// Локальное имя не подхдодит для связывания
static char *md1_local(void)
{
    return md1_data;
}


// Не объялвено как экспортируемое имя и не может быть
// использовано вне модуля
extern char *md1_noexport(void)
{
    return md1_data;
}


// Любой другой модуль может использовать
// в своем коде любые экспортируемые имена

// EXPORT_SYMBOL() - дает возможность предоставить
// API для других модулей или кода

// Модуль не будет загружен, если он ожидает переменную или
// функцию, которой нет в ядре 
EXPORT_SYMBOL(md1_data);
EXPORT_SYMBOL(md1_proc);


static int __init md_init(void)
{
    // + для поиска информации от модуля через grep
    printk(KERN_INFO "+ Module md1 started\n");
    printk(KERN_INFO "+ Module md1 use local from md1: %s\n", md1_local());
    printk(KERN_INFO "+ Module md1 use noexport from md1: %s\n", md1_noexport());
    return 0;
}


static void __exit md_exit(void)
{
    printk(KERN_INFO "+ Module md1 unloaded\n");
}

module_init(md_init);
module_exit(md_exit);
