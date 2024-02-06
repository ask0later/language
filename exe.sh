cd ..
cd Processor/
cd asm
make
./asm ../../language/examples/$1.asm ../../language/binary.bin
cd ..
cd spu
make
./spu ../../language/binary.bin