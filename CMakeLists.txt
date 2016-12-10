cmake_minimum_required(VERSION 2.4)
PROJECT(linux_esp32)
#SET (CMAKE_BUILD_TYPE Release)
SET (CMAKE_BUILD_TYPE Debug)



enable_testing()

set(IDF_PATH /home/olas/esp/esp-idf)
set(LDF_PATH /home/olas/esp/linux_esp32)
set(THIS_PATH /home/olas/esp/esp32_awho)

ADD_DEFINITIONS(-D TEST )

set(CMAKE_CXX_FLAGS "-g -Wall -std=c++11 -static-libstdc++")
set(CMAKE_C_FLAGS "-g -std=gnu99")


INCLUDE_DIRECTORIES(
${IDF_PATH}/components/nvs_flash/include/
${IDF_PATH}/components/esp32/include/
${IDF_PATH}/components/esp32/include
${IDF_PATH}/components/driver/include/
${IDF_PATH}/components/log/include/
${IDF_PATH}/components/tcpip_adapter/include
${IDF_PATH}/components/tcpip_adapter/include
${LDF_PATH}/components/lwip/include
${LDF_PATH}/components/lwip/include/lwip/port
${IDF_PATH}/components/lwip/include/lwip
${LDF_PATH}/freeRTOS
${LDF_PATH}/freeRTOS/freertos
${IDF_PATH}/components/lwip/port/include
${IDF_PATH}/components/lwip/include/lwip
${IDF_PATH}/components/lwip/include/lwip/port
${LDF_PATH}/freeRTOS/portable/GCC/Posix
${THIS_PATH}/build/include/
)

INCLUDE_DIRECTORIES(BEFORE
${LDF_PATH}/components/lwip/include
${LDF_PATH}/components/ethernet/include
${LDF_PATH}/components/lwip/include/lwip
${LDF_PATH}/components/lwip/port/include
${LDF_PATH}/components/lwip/port
${LDF_PATH}/components/lwip
)


SET (LIBRARY_TYPE STATIC)

ADD_LIBRARY(dummy
${LDF_PATH}/components/nvs_flash/nvs_flash.c
${LDF_PATH}/components/esp32/dummy_esp32.c
${LDF_PATH}/components/esp32/dummy_esp_wifi.c
${LDF_PATH}/components/esp32/dummy_esp_wifi_internal.c
)

ADD_LIBRARY(freertos
${LDF_PATH}/freeRTOS/croutine.c
${LDF_PATH}/freeRTOS/list.c
${LDF_PATH}/freeRTOS/queue.c
${LDF_PATH}/freeRTOS/tasks.c
${LDF_PATH}/freeRTOS/portable/MemMang/heap_3.c
${LDF_PATH}/freeRTOS/portable/GCC/Posix/port.c
${LDF_PATH}/freeRTOS/freertos/croutine.h
${LDF_PATH}/freeRTOS/freertos/FreeRTOS.h
${LDF_PATH}/freeRTOS/freertos/list.h
${LDF_PATH}/freeRTOS/freertos/mpu_wrappers.h
${LDF_PATH}/freeRTOS/freertos/portable.h
${LDF_PATH}/freeRTOS/freertos/projdefs.h
${LDF_PATH}/freeRTOS/freertos/queue.h
${LDF_PATH}/freeRTOS/freertos/semphr.h
${LDF_PATH}/freeRTOS/freertos/StackMacros.h
${LDF_PATH}/freeRTOS/freertos/task.h
)

set(LWIP_SRC
${IDF_PATH}/components/lwip/core/def.c          
${IDF_PATH}/components/lwip/core/mem.c    
${IDF_PATH}/components/lwip/core/stats.c    
${IDF_PATH}/components/lwip/core/timers.c
${IDF_PATH}/components/lwip/core/dns.c          
${IDF_PATH}/components/lwip/core/memp.c   
${IDF_PATH}/components/lwip/core/sys.c      
${IDF_PATH}/components/lwip/core/udp.c
${IDF_PATH}/components/lwip/core/inet_chksum.c  
${IDF_PATH}/components/lwip/core/netif.c  
${IDF_PATH}/components/lwip/core/tcp.c
${IDF_PATH}/components/lwip/core/init.c         
${IDF_PATH}/components/lwip/core/pbuf.c   
${IDF_PATH}/components/lwip/core/tcp_in.c
${IDF_PATH}/components/lwip/core/ip.c           
${IDF_PATH}/components/lwip/core/raw.c    
${IDF_PATH}/components/lwip/core/tcp_out.c
${IDF_PATH}/components/lwip/core/ipv4/autoip.c
${IDF_PATH}/components/lwip/core/ipv4  
${IDF_PATH}/components/lwip/core/ipv4/dhcp.c  
${IDF_PATH}/components/lwip/core/ipv4/icmp.c  
${IDF_PATH}/components/lwip/core/ipv4/igmp.c  
${IDF_PATH}/components/lwip/core/ipv4/ip4_addr.c  
${IDF_PATH}/components/lwip/core/ipv4/ip4.c  
${IDF_PATH}/components/lwip/core/ipv4/ip_frag.c
${IDF_PATH}/components/tcpip_adapter/tcpip_adapter_lwip.c
${IDF_PATH}/components/lwip/netif/etharp.c    
${IDF_PATH}/components/lwip/netif/ethernetif.c
${IDF_PATH}/components/lwip/netif/ethernet.c
${IDF_PATH}/components/lwip/apps/dhcpserver.c
${IDF_PATH}/components/lwip/core/ipv6/dhcp6.c   
${IDF_PATH}/components/lwip/core/ipv6/ip6_addr.c  
${IDF_PATH}/components/lwip/core/ipv6/nd6.c
${IDF_PATH}/components/lwip/core/ipv6/ethip6.c  
${IDF_PATH}/components/lwip/core/ipv6/ip6.c       
${IDF_PATH}/components/lwip/core/ipv6/icmp6.c   
${IDF_PATH}/components/lwip/core/ipv6/ip6_frag.c
${IDF_PATH}/components/lwip/core/ipv6/inet6.c   
${IDF_PATH}/components/lwip/core/ipv6/mld6.c
${IDF_PATH}/components/lwip/api/tcpip.c
${IDF_PATH}/components/lwip/api/api_lib.c  
${IDF_PATH}/components/lwip/api/err.c         
${IDF_PATH}/components/lwip/api/netbuf.c  
${IDF_PATH}/components/lwip/api/netifapi.c  
${IDF_PATH}/components/lwip/api/sockets.c
${IDF_PATH}/components/lwip/api/api_msg.c  
${IDF_PATH}/components/lwip/api/lwip_debug.c  
#${IDF_PATH}/components/lwip/api/netdb.c   
${IDF_PATH}/components/lwip/api/pppapi.c    
${LDF_PATH}/components/lwip/port/dummy_sys_arch.c
${LDF_PATH}/components/lwip/port/netif/tapif.c
${LDF_PATH}/linux_lwip.c
)

ADD_EXECUTABLE(awho
main/awho.c
main/echo.c
${LWIP_SRC}
${LDF_PATH}/main_task.c
${IDF_PATH}/components/log/log.c
${IDF_PATH}/components/esp32/event_loop.c
${IDF_PATH}/components/esp32/event_default_handlers.c
)

TARGET_LINK_LIBRARIES(awho freertos dummy pthread)


#add_test(test1 testFunctions)


ADD_DEFINITIONS(-D NOTHING_To_DEFINE_FOR_NOW )

