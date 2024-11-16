# temporary build script
rm threeNPlusOne.o threeNPlusOne.x86_64
cc -c -o threeNPlusOne.o -m32 -Wextra -Wall -Werror -ansi --std=c99 ../lib/threeNPlusOne/threeNPlusOne.c
cc -o threeNPlusOne.x86_64 -m32 -Wextra -Wall -Werror -ansi --std=c99 threeNPlusOne.c threeNPlusOne.o
./threeNPlusOne.x86_64 10
