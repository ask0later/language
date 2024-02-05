cd ..
cd Processor/
cd asm
./asm ../../language/examples/$1.asm ../../language/binary.bin
cd ..
cd spu
./spu ../../language/binary.bin