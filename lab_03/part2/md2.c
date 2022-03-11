// Вызывающий модуль ядра
// Использует экспортируемое имя, с которым связывается
// по абсолютному адресу

#include <linux/init.h>
#include <linux/module.h>

#include "md.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_03 (Part 2): md2");


static int __init md_init(void)
{
	printk("+ Module md2 start!\n");
	printk("+ md2: data string exported from md1 : %s\n", md1_data);
	printk("+ md2: string returned md1_proc() is : %s\n", md1_proc());

    // Не компилируется 
    // printk( "+ md2: module md2 use local from md1: %s\n", md1_local());

    // printk( "+ md2: module md2 use noexport from md1: %s\n", md1_noexport());	
	return 0;
}


static void __exit md_exit(void)
{
	printk("+ Module md2 unloaded\n");
}

module_init(md_init);
module_exit(md_exit);
