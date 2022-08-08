#include <linux/init.h>
#include <linux/module.h>

static int __init mycdev_init(void)
{
    
    return EIO;
}

static void __exit mycdev_exit(void)
{
    
    
    
}
module_init(mycdev_init);
module_exit(mycdev_exit);
MODULE_LICENSE("GPL");