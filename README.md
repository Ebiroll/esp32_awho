# esp32_awho
Esp32 arp-ping to see who is connected to the wifi router. 

#How 
Developped with qemu, this is how i tested.

xtensa-softmmu/qemu-system-xtensa -d guest_errors,unimp   -cpu esp32 -M esp32 -m 4M -net nic,model=vlan0 -net user,id=simnet,ipver4=on,net=192.168.1.0/24,host=192.168.1.40,hostfwd=tcp::10023-192.168.1.3:23  -net dump,file=/tmp/vm0.pcap  -kernel  ~/esp/esp32_awho/build/awho.elf -s  > io.txt


nc 127.0.0.1 10023