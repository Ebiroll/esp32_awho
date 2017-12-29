# esp32_awho
Esp32 arp-ping to see who is connected to the wifi router. 

#How 
Developped with qemu, this is how i tested.


#To start qemu
xtensa-softmmu/qemu-system-xtensa -d guest_errors,unimp   -cpu esp32 -M esp32 -m 4M -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10023-192.168.1.3:23  -net dump,file=/tmp/vm0.pcap  -kernel  ~/esp/esp32_awho/build/awho.elf -s  > io.txt

First I did not get interrupts when using -smp 2 because emulated interrupts dont work as I expected.
I fixed that.
xtensa-softmmu/qemu-system-xtensa -d guest_errors,unimp  -cpu esp32 -M esp32 -m 4M -smp 2  -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10023-192.168.1.3:23  -net dump,file=/tmp/vm0.pcap  -kernel  ~/esp/esp32_awho/build/awho.elf  -s  -S > io.txt

#To test echo
xtensa-softmmu/qemu-system-xtensa  -d guest_errors   -cpu esp32 -M esp32 -m 4M -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10007-192.168.1.3:7  -net dump,file=/tmp/vm0.pcap  -kernel   ~/esp/esp32_awho/build/awho.elf -s  > io.txt


#gdb 
Run from within code ide which uses .vscode/launch.json or
xtensa-esp32-elf-gdb build/awho.elf  -ex 'target remote:1234'


#$HOME/.gdbinit
To get rom adresses resolved add to your ~/.gdbinit
add-auto-load-safe-path /home/olas/esp/esp32_awho/.gdbinit

#In this directory/.gdbinit
add-symbol-file rom.elf 0x40000000


#Todo for qemu
We need better SPI emulation.
Analyze static load_partitions() 
spi_flash_munmap()


Here is some analysis snippets.
void esp_crosscore_int_init() {
        portENTER_CRITICAL(&reasonSpinlock);
        reason[xPortGetCoreID()]=0;
        portEXIT_CRITICAL(&reasonSpinlock);
        ESP_INTR_DISABLE(ETS_FROM_CPU_INUM);
        if (xPortGetCoreID()==0) {
            intr_matrix_set(xPortGetCoreID(), ETS_FROM_CPU_INTR0_SOURCE, ETS_FROM_CPU_INUM);
        } else {
            intr_matrix_set(xPortGetCoreID(), ETS_FROM_CPU_INTR1_SOURCE, ETS_FROM_CPU_INUM);
        } 
        xt_set_interrupt_handler(ETS_FROM_CPU_INUM, esp_crosscore_isr, (void*)&reason[xPortGetCoreID()]);
        ESP_INTR_ENABLE(ETS_FROM_CPU_INUM);
}                


0x40084a00 <xt_ints_off>        entry  a1, 16  
0x40084a03 <xt_ints_off+3>      movi.n a3, 0 
0x40084a05 <xt_ints_off+5>      l32r   a4, 0x400808c0
0x40084a08 <xt_ints_off+8>      xsr.intenable  a3
0x40084a0b <xt_ints_off+11>     rsync
0x40084a0e <xt_ints_off+14>     l32i.n a3, a4, 0
0x40084a10 <xt_ints_off+16>     l32i.n a6, a4, 4
0x40084a12 <xt_ints_off+18>     or     a5, a3, a2
0x40084a15 <xt_ints_off+21>     xor    a5, a5, a2
0x40084a18 <xt_ints_off+24>     s32i.n a5, a4, 0 
0x40084a1a <xt_ints_off+26>     and    a5, a5, a6 
(gdb) p/x $a5
$4 = 0x1400008
0x40084a1d <xt_ints_off+29>     wsr.intenable  a5
0x40084a20 <xt_ints_off+32>     mov.n  a2, a3
0x40084a22 <xt_ints_off+34>     retw.n


nc 127.0.0.1 10023
