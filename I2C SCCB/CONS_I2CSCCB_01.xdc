#set_property IOSTANDARD LVCMOS33 [get_ports CAMPWDN]
#set_property IOSTANDARD LVCMOS33 [get_ports CAMRST]

set_property IOSTANDARD LVCMOS33 [get_ports iic_rtl_0_scl_io]
set_property IOSTANDARD LVCMOS33 [get_ports iic_rtl_0_sda_io]
set_property PACKAGE_PIN T19 [get_ports iic_rtl_0_scl_io]
set_property PACKAGE_PIN U20 [get_ports iic_rtl_0_sda_io]

set_property PULLUP true [get_ports iic_rtl_0_scl_io]
set_property PULLUP true [get_ports iic_rtl_0_sda_io]

set_property IOSTANDARD LVCMOS33 [get_ports {GPIO_0_0_tri_io[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {GPIO_0_0_tri_io[0]}]
set_property PACKAGE_PIN U18 [get_ports {GPIO_0_0_tri_io[0]}]
set_property PACKAGE_PIN V20 [get_ports {GPIO_0_0_tri_io[1]}]
#set_property PACKAGE_PIN T10 [get_ports {GPIO_0_0_tri_io[0]}]
#set_property PACKAGE_PIN T11 [get_ports {GPIO_0_0_tri_io[1]}]

