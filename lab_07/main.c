#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/init.h>  
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/time.h>



MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tsvetkov IU7-63B");
MODULE_DESCRIPTION("LAB_07: File System");



#define A23FS_MAGIC 0xDEADBEEF
#define SLAB_NAME "a23fs_slab"


struct myfs_inode
{
    int i_mode;
    unsigned long i_ino;
} myfs_inode;

static int sco = 0;
static int number = 31;
struct kmem_cache *cache = NULL; 
static void **line = NULL;
static int size = sizeof(struct myfs_inode);


void co(void* p)
{ 
    *(int*)p = (int)p; 
    sco++; 
}


static struct inode *myfs_make_inode(struct super_block *sb, int mode)
{
    struct inode *ret = new_inode(sb);

    if (ret)
    {
        inode_init_owner(&init_user_ns, ret, NULL, mode);

        ret->i_size = PAGE_SIZE;
        ret->i_atime = ret->i_mtime = ret->i_ctime = current_time(ret);
        ret->i_private = &myfs_inode;
    }

    printk(KERN_INFO "+: a23fs - new inode was creared\n");

    return ret;
}


static void myfs_put_super(struct super_block *sb)
{
    printk(KERN_INFO "+: a23fs - super block was destroyed\n");
}


static struct super_operations const myfs_super_ops = {
    .put_super = myfs_put_super,
    .statfs = simple_statfs,
    .drop_inode = generic_delete_inode,
};


static int myfs_fill_sb(struct super_block *sb, void *data, int silent)
{
    struct inode *root = NULL;

    // Инициализация полей структуры суперблока
    sb->s_blocksize = PAGE_SIZE;
    sb->s_blocksize_bits = PAGE_SHIFT;
    sb->s_op = &myfs_super_ops;

    // по данному числу драйвер сможет проверить наличие
    // на диске данной файловой системы
    sb->s_magic = A23FS_MAGIC;

    // Корневой каталог Файловой системы
    // S_IFDIR - создаем каталог.
	// 0755 - стандартные права доступа для папок
    root = myfs_make_inode(sb, S_IFDIR | 0755);

    if (!root)
    {
        printk(KERN_ERR "+: a23fs inode allocation failed\n");

        return -ENOMEM;
    }

    root->i_op = &simple_dir_inode_operations;
    root->i_fop = &simple_dir_operations;
    sb->s_root = d_make_root(root); // dentry корневого каталога

    if (!sb->s_root)
    {
        printk(KERN_ERR "+: a23fs root creation failed\n");
        iput(root);

        return -ENOMEM;
    }

    printk(KERN_INFO "+: a23fs - root was creared\n");

    return 0;
}


static struct dentry *myfs_mount(struct file_system_type *type, int flags, char const *dev, void *data)
{
    // Монитруется устройство
    // Возвращается структура, описывающая корневой каталог ФС
    // myfs_fill_sb - проинициализирует суперблок
    struct dentry *const entry = mount_nodev(type, flags, data, myfs_fill_sb); // VFS

    if (IS_ERR(entry))
    {
        printk(KERN_ERR "+: a23fs mounting failed\n");
    }
    else
    {
        printk(KERN_DEBUG "+: a23fs mounted\n");
    }

    return entry;
}


static void myfs_kill_anon_super(struct super_block *sb)
{
    printk(KERN_INFO "+: a23fs - My kill_anon_super\n");

    return kill_anon_super(sb);
}




static struct file_system_type myfs_type = {
    .owner = THIS_MODULE,
    .name = "a23fs",
    .mount = myfs_mount,
    .kill_sb = myfs_kill_anon_super, // при размонтировании ФС
};


static int __init myfs_init(void)
{
    line = kmalloc(sizeof(void*), GFP_KERNEL); 
    if(!line)
    { 
        printk(KERN_ERR "+: kmalloc error\n"); 
        return -ENOMEM;
    }
 

	cache = kmem_cache_create(SLAB_NAME, size, 0, 0, co); 
    if(!cache) 
    { 
        printk(KERN_ERR "+: kmem_cache_create error\n"); 
        kfree(line); 
	    return -ENOMEM;  
    } 


    if (!((*line) = kmem_cache_alloc(cache, GFP_KERNEL)))
    {
        printk(KERN_ERR "+: kmem_cache_alloc error\n"); 

        kmem_cache_free(cache, *line);
        kmem_cache_destroy(cache);
        kfree(line);

        return -ENOMEM;
    }


    int ret = register_filesystem(&myfs_type);

    if (ret != 0)
    {
        printk(KERN_ERR "+: a23fs module cannot register filesystem\n");

        return ret;
    }



    printk(KERN_INFO "+: a23fs_module loaded\n");

    return 0;
}


static void __exit myfs_exit(void)
{
    kmem_cache_free(cache, *line);
    kmem_cache_destroy(cache); 
    kfree(line);

    int ret = unregister_filesystem(&myfs_type);

    if (ret != 0)
    {
        printk(KERN_ERR "+: a23fs_module cannot unregister filesystem\n");
    }

    printk(KERN_DEBUG "+: a23fs_module unloaded\n");
}

module_init(myfs_init);
module_exit(myfs_exit);