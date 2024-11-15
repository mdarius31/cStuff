# temporary build script
cc -c -o threeNPlusOne.o -Wextra -Wall -Werror -ansi --std=c99 ../lib/threeNPlusOne/threeNPlusOne.c
cc -o threeNPlusOne.x86_64 -Wextra -Wall -Werror -ansi --std=c99 threeNPlusOne.c threeNPlusOne.o && ./threeNPlusOne.x86_64
