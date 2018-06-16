#/usr/bin/!sh
binary=$1

echo "$binary"
objdump -S $binary > $binary".obj"
hexdump -C $binary > $binary".hex"
