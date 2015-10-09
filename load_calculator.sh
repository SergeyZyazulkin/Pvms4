insmod calculator.ko || exit
mknod /dev/calculator_first c $(awk '$2=="calculator_first" {print $1}' /proc/devices) 0
mknod /dev/calculator_second c $(awk '$2=="calculator_second" {print $1}' /proc/devices) 0
mknod /dev/calculator_operation c $(awk '$2=="calculator_operation" {print $1}' /proc/devices) 0
mknod /dev/calculator_result c $(awk '$2=="calculator_result" {print $1}' /proc/devices) 0
chmod 666 /dev/calculator_first
chmod 666 /dev/calculator_second
chmod 666 /dev/calculator_operation
chmod 444 /dev/calculator_result
