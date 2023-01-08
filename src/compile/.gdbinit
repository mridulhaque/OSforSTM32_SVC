set auto-load safe-path /
echo Executing GDB with .gdbinit to connect to OpenOCD.\n
echo .gdbinit is a hidden file. Press Ctrl-H in the current working directory to see it.\n
# Connect to OpenOCD
target remote localhost:3333
# Reset the target and call its init script
monitor reset init
# Halt the target. The init script should halt the target, but just in case
monitor halt
# load the elf file
monitor flash write_image erase target/asst03
# reset
monitor reset
# quit
q
