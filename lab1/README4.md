# Лабораторная работа 1

**Название:** "Разработка драйверов символьных устройств"

**Цель работы:** Write a character device driver that meets the requirements

## Описание функциональности драйвера

When writing a string to the character device /dev/var4, count the number of spaces and keep it consistent with the content of /proc/var4

## Инструкция по сборке

1. Enter the source program directory
2. execute make
3. execute insmod var4.ko
4. View device number: 
5. Create a device node in the /dev directory according to the device number
  

## Инструкция пользователя

1. write test program
2. Read and write the /dev/var4 device driver file in the test program.
3. echo ** > /dev/var4
4.cat /proc/var4

## Примеры использования

