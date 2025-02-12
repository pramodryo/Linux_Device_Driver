# Linux Device Driver

/***********************************************************************/ 
compile driver code: Make
compile application : gcc -o test_app application.c
load module into kernel: sudo insmod driver.ko
check message: dmesg
To check running module in kernel: lsmod   (lsmod | gerp driver)
To remove current running module from kernel: sudo rmmod driver

