$ gdb --args ls
...
(gdb) set environment LD_PRELOAD=/home/joel/mymalloc/libmymalloc.so
(gdb) b MallocWrapper.cpp:malloc
...
(gdb) r
Starting program: /usr/bin/ls
Breakpoint 1, malloc (size=73728) at src/MallocWrapper.cpp:44
...
(gdb) bt
#0  malloc (size=73728) at src/MallocWrapper.cpp:44
#1  0x00007ffff78bd17f in ?? () from /lib/x86_64-linux-gnu/libstdc++.so.6
#2  0x00007ffff7fca71f in call_init (l=<optimized out>, argc=argc@entry=1, argv=argv@entry=0x7fffffffdce8, env=env@entry=0x7fffffffdcf8) at ./elf/dl-init.c:74
#3  0x00007ffff7fca824 in call_init (env=<optimized out>, argv=<optimized out>, argc=<optimized out>, l=<optimized out>) at ./elf/dl-init.c:120
#4  _dl_init (main_map=0x7ffff7ffe2e0, argc=1, argv=0x7fffffffdce8, env=0x7fffffffdcf8) at ./elf/dl-init.c:121
#5  0x00007ffff7fe45a0 in _dl_start_user () from /lib64/ld-linux-x86-64.so.2
# ...
