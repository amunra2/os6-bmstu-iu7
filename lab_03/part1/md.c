#include <linux/init_task.h> // init_task
#include <linux/kernel.h>    // уровни протоколирования, printk
#include <linux/module.h>    // макросы module_init, module_exit, MODULE_LICENSE
#include <linux/sched.h>     // task_struct


// Лицензия для модуля (влияет на то, к каким функциям и переменным
// может получить доступ)
MODULE_LICENSE("GPL");
// Информация об авторе модуля
MODULE_AUTHOR("Tsvetkov IU7-63B");
// Информация о модуле
MODULE_DESCRIPTION("LAB_03 (Part 1)");


// int func_unit(void) - успех = 0, ошибка - иначе
// static - не вызывается за пределами модуля
// __init - используется на этапе инициализации
static int __init md_init(void) 
{
    struct task_struct *cur_task = &init_task;


    // Отправляет сообщения в системный журнал.
    // Записывает сообщение в специальный буфер ядра, откуда их
    // может прочесть демон протоколирования
    // KERN_INFO - уровень информационного сообщения
    printk(KERN_INFO "LAB_03 (Part 1): module is loaded.\n");

    do
    {
        printk(KERN_INFO "LAB_03 (Part 1): name: %-20s\t id: %-9d state: %-7ld static_prio: %-7d; parent name: %-20s\t id: %-5d",
               cur_task->comm, cur_task->pid, cur_task->state, cur_task->static_prio, cur_task->parent->comm, cur_task->parent->pid);

    } while ((cur_task = next_task(cur_task)) != &init_task);

    // current - определяет текущий процесс
    printk(KERN_INFO "LAB_03 (Part 1): This name: %-20s\t id: %-9d state: %-7ld static_prio: %-7d; parent name: %-20s\t id: %-5d",
           current->comm, current->pid, current->state, current->static_prio, current->parent->comm, current->parent->pid);

    return 0;
}



// void func_exit(void)
static void __exit md_exit(void)
{
    printk(KERN_INFO "LAB_03 (Part 1): module is unloaded.\n");
}


// Регистрация функции инициализации модуля
// Запрос ресурсов и выделение памяти под структуры
module_init(md_init);


// Регистрация функции, вызывающейся при удалении модуля из ядра
// Освобождение ресурсов
module_exit(md_exit);
