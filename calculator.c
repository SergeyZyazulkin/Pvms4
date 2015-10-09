#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <asm/uaccess.h>


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Zyazulkin Sergey");


#define NUMBER_SIZE 12
#define OPERATION_SIZE 2
#define RESULT_SIZE 50


static const char calculator_first_name[] = "calculator_first";
static const char calculator_second_name[] = "calculator_second";
static const char calculator_operation_name[] = "calculator_operation";
static const char calculator_result_name[] = "calculator_result";

static int calculator_first_major = -1;
static int calculator_second_major = -1;
static int calculator_operation_major = -1;
static int calculator_result_major = -1;

static char calculator_first_value[NUMBER_SIZE] = { 0 };
static char calculator_second_value[NUMBER_SIZE] = { 0 };
static char calculator_operation_value[OPERATION_SIZE] = { 0 };
static char calculator_result_value[RESULT_SIZE] = { 0 };


static int calculator_init(void);
static void calculator_exit(void);

static ssize_t calculator_read_first(struct file*, char*, size_t, loff_t*);
static ssize_t calculator_read_second(struct file*, char*, size_t, loff_t*);
static ssize_t calculator_read_operation(struct file*, char*, size_t, loff_t*);
static ssize_t calculator_read_result(struct file*, char*, size_t, loff_t*);

static ssize_t calculator_write_first(struct file*, const char*, size_t, loff_t*);
static ssize_t calculator_write_second(struct file*, const char*, size_t, loff_t*);
static ssize_t calculator_write_operation(struct file*, const char*, size_t, loff_t*);

static void calculate(void);


static struct file_operations calculator_first_fops = {
    .owner = THIS_MODULE,
    .read = calculator_read_first,
    .write = calculator_write_first,
};

static struct file_operations calculator_second_fops = {
    .owner = THIS_MODULE,
    .read = calculator_read_second,
    .write = calculator_write_second,
};

static struct file_operations calculator_operation_fops = {
    .owner = THIS_MODULE,
    .read = calculator_read_operation,
    .write = calculator_write_operation,
};

static struct file_operations calculator_result_fops = {
    .owner = THIS_MODULE,
    .read = calculator_read_result,
};


static int calculator_init()
{
    calculator_first_major = register_chrdev(0, calculator_first_name, &calculator_first_fops);

    if (calculator_first_major < 0) {
        calculator_exit();
        printk("error: could not register calculator_first\n");
        return calculator_first_major;
    }

    printk("calculator_first has been registered\n");

    calculator_second_major = register_chrdev(0, calculator_second_name, &calculator_second_fops);

    if (calculator_second_major < 0) {
        calculator_exit();
        printk("error: could not register calculator_second\n");
        return calculator_second_major;
    }

    printk("calculator_second has been registered\n");

    calculator_operation_major = register_chrdev(0, calculator_operation_name, &calculator_operation_fops);

    if (calculator_operation_major < 0) {
        calculator_exit();
        printk("error: could not register calculator_operation\n");
        return calculator_operation_major;
    }

    printk("calculator_operation has been registered\n");

    calculator_result_major = register_chrdev(0, calculator_result_name, &calculator_result_fops);

    if (calculator_result_major < 0) {
        calculator_exit();
        printk("error: could not register calculator_result\n");
        return calculator_result_major;
    }

    printk("calculator_result has been registered\n");

    return 0;
}

static void calculator_exit()
{
    if (calculator_first_major > 0) {
        unregister_chrdev(calculator_first_major, calculator_first_name);
    }

    if (calculator_second_major > 0) {
        unregister_chrdev(calculator_second_major, calculator_second_name);
    }

    if (calculator_operation_major > 0) {
        unregister_chrdev(calculator_operation_major, calculator_operation_name);
    }

    if (calculator_result_major > 0) {
        unregister_chrdev(calculator_result_major, calculator_result_name);
    }
}


static ssize_t calculator_read_first(struct file* filp, char* buff, size_t count, loff_t* offset)
{
    if (*offset >= NUMBER_SIZE) {
        return 0;
    }

    if (*offset + count > NUMBER_SIZE) {
        count = NUMBER_SIZE - *offset;
    }

    if (copy_to_user(buff, calculator_first_value + *offset, count) != 0) {
        printk("error: could not copy to user\n");
        return -EINVAL;
    }

    printk("first has been read\n");
    *offset += count;
    return count;
}

static ssize_t calculator_read_second(struct file* filp, char* buff, size_t count, loff_t* offset)
{
    if (*offset >= NUMBER_SIZE) {
        return 0;
    }

    if (count > NUMBER_SIZE) {
        count = NUMBER_SIZE - *offset;
    }

    if (copy_to_user(buff, calculator_second_value + *offset, count) != 0) {
        printk("error: could not copy to user\n");
        return -EINVAL;
    }

    printk("second has been read\n");
    *offset += count;
    return count;
}

static ssize_t calculator_read_operation(struct file* filp, char* buff, size_t count, loff_t* offset)
{
    if (*offset >= OPERATION_SIZE) {
        return 0;
    }

    if (count > OPERATION_SIZE) {
        count = OPERATION_SIZE - *offset;
    }

    if (copy_to_user(buff, calculator_operation_value + *offset, count) != 0) {
        printk("error: could not copy to user\n");
        return -EINVAL;
    }

    printk("operation has been read\n");
    *offset += count;
    return count;
}

static void calculate()
{
    long first_attribute, second_attribute;
    memset(calculator_result_value, 0, sizeof(calculator_result_value));

    if (kstrtol(calculator_first_value, 10, &first_attribute) != 0) {
        snprintf(calculator_result_value, RESULT_SIZE, "invalid first attribute");
        return;
    }

    if (kstrtol(calculator_second_value, 10, &second_attribute) != 0) {
        snprintf(calculator_result_value, RESULT_SIZE, "invalid second attribute");
        return;
    }

    switch (calculator_operation_value[0]) {
        case '+':
            snprintf(calculator_result_value, RESULT_SIZE, "%ld", first_attribute + second_attribute);
            return;

        case '-':
            snprintf(calculator_result_value, RESULT_SIZE, "%ld", first_attribute - second_attribute);
            return;

        case '*':
            snprintf(calculator_result_value, RESULT_SIZE, "%ld", first_attribute * second_attribute);
            return;

        case '/':
            if (second_attribute == 0) {
                snprintf(calculator_result_value, RESULT_SIZE, "division by zero");
                return;
            }

            snprintf(calculator_result_value, RESULT_SIZE, "%ld", first_attribute / second_attribute);
            return;

        default:
            snprintf(calculator_result_value, RESULT_SIZE, "invalid operation");
    }
}

static ssize_t calculator_read_result(struct file* filp, char* buff, size_t count, loff_t* offset)
{
    if (*offset >= RESULT_SIZE) {
        return 0;
    }

    if (count > RESULT_SIZE) {
        count = RESULT_SIZE - *offset;
    }

    calculate();

    if (copy_to_user(buff, calculator_result_value + *offset, count) != 0) {
        printk("error: could not copy to user\n");
        return -EINVAL;
    }

    printk("result has been read\n");
    *offset += count;
    return count;
}


static ssize_t calculator_write_first(struct file* filp, const char* buff, size_t count, loff_t* offset)
{
    if (count > NUMBER_SIZE) {
        printk("error: too big data\n");
        return -EINVAL;
    }

    memset(calculator_first_value, 0, sizeof(calculator_first_value));

    if (copy_from_user(calculator_first_value, buff, count) != 0) {
        printk("error: could not copy from user\n");
        return -EINVAL;
    }

    printk("first has been written\n");
    return count;
}

static ssize_t calculator_write_second(struct file* filp, const char* buff, size_t count, loff_t* offset)
{
    if (count > NUMBER_SIZE) {
        printk("error: too big data\n");
        return -EINVAL;
    }

    memset(calculator_second_value, 0, sizeof(calculator_second_value));

    if (copy_from_user(calculator_second_value, buff, count) != 0) {
        printk("error: could not copy from user\n");
        return -EINVAL;
    }

    printk("second has been written\n");
    return count;
}

static ssize_t calculator_write_operation(struct file* filp, const char* buff, size_t count, loff_t* offset)
{
    if (count > OPERATION_SIZE) {
        printk("error: too big data\n");
        return -EINVAL;
    }

    memset(calculator_operation_value, 0, sizeof(calculator_operation_value));

    if (copy_from_user(calculator_operation_value, buff, count) != 0) {
        printk("error: could not copy from user\n");
        return -EINVAL;
    }

    printk("operation has been written\n");
    return count;
}


module_init(calculator_init);
module_exit(calculator_exit);
