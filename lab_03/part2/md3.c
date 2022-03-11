#include <linux/init.h>
#include <linux/module.h>

#include "md.h"


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_03 (Part 2): md3");


static int __init md_init(void)
{
	printk("+ Module md3 started\n");
	printk("+ md3: data string exported from md1 : %s\n", md1_data);
	printk("+ md3: string returned md1_proc() is : %s\n", md1_proc());

    // Не компилируется
	// printk("+ md3: try to call md1_noexport\n");
	// printk("+ string returned md1_noexport() is : %s\n", md1_noexport());
	// printk("+ md1_noexport был вызван\n");

	// printk("+ md3: try to call md1_local\n");
	// printk("+ md3: string returned md1_local() is : %s\n", md1_local());
	// printk("+ md3: md1_local был вызван\n");


	// Возвращает код ошибки, что приводит к тому, что модуль не будет загружен в ядро.
	// Но произойдёт это уже после выполнения кода инициализирующей функции модуля в пространстве ядра
	return -1;
}

module_init(md_init);

// Если такой модуль по замыслу не загружается,
// то он, в принципе, может не иметь функции выгрузки.
