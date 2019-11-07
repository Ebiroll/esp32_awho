#idf_component_register(SRCS "emul_ip.c" "lwip_ethoc.c"
#                       INCLUDE_DIRS ".")
set(COMPONENT_SRCS "libtelnet.c")
set(COMPONENT_ADD_INCLUDEDIRS "include" ".")

register_component()
