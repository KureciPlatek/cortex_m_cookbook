#source gdbtrace.init
target extended-remote /dev/ttyBmpGdb
monitor connect_rst enable
monitor swdp_scan
file ./bin/prj_h7.elf
attach 1
set mem inaccessible-by-default off
set print pretty
load
