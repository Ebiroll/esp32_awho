# esp32_awho
Esp32 arp-ping to see who is connected to the wifi router. 

    nc 192.168.1.144 23
Press return to see all connected devices



# How 
Developped with qemu, this is how i tested.


# To start qemu
    xtensa-softmmu/qemu-system-xtensa -d guest_errors,unimp   -cpu esp32 -M esp32 -m 4M -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10023-192.168.1.3:23  -net dump,file=/tmp/vm0.pcap  -s  > io.txt



    xtensa-softmmu/qemu-system-xtensa -d guest_errors,unimp  -cpu esp32 -M esp32 -m 4M -smp 2  -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10023-192.168.1.3:23  -net dump,file=/tmp/vm0.pcap    -s  -S > io.txt

    nc 127.0.0.1 10023

# To test echo
    xtensa-softmmu/qemu-system-xtensa  -d guest_errors   -cpu esp32 -M esp32 -m 4M -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10007-192.168.1.3:7  -net dump,file=/tmp/vm0.pcap   -s  > io.txt


# gdb 
Run from within code ide which uses .vscode/launch.json or
    xtensa-esp32-elf-gdb build/awho.elf  -ex 'target remote:1234'


# $HOME/.gdbinit
To get rom adresses resolved add to your ~/.gdbinit
add-auto-load-safe-path /home/olas/esp/esp32_awho/.gdbinit

#In this directory/.gdbinit
add-symbol-file rom.elf 0x40000000



nc 127.0.0.1 10023
