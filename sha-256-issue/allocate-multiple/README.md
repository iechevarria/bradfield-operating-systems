In comparison to `reuse`, we allocate 5 different hash arrays and noticed that the answer is consistent between calls (although the output is still different between runs of the actual program).

Why does allocating multiple arrays ahead of time "fix" the issue in `reuse`?

```bash
for i in {1..5}; do echo "run $i"; ./run.sh; echo; done;
```

Output:
```
run 1
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 5d1e6df5b2a4669885970039463251e04967023724b807145c952cf05e3e573d
sha256(2) = 5d1e6df5b2a4669885970039463251e04967023724b807145c952cf05e3e573d
sha256(2) = 5d1e6df5b2a4669885970039463251e04967023724b807145c952cf05e3e573d
sha256(2) = 5d1e6df5b2a4669885970039463251e04967023724b807145c952cf05e3e573d
sha256(2) = 5d1e6df5b2a4669885970039463251e04967023724b807145c952cf05e3e573d

run 2
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 0d995f325fcf59d23ace49a0a08cbf1504fff5e677381878e634bb7ab559c3c5
sha256(2) = 0d995f325fcf59d23ace49a0a08cbf1504fff5e677381878e634bb7ab559c3c5
sha256(2) = 0d995f325fcf59d23ace49a0a08cbf1504fff5e677381878e634bb7ab559c3c5
sha256(2) = 0d995f325fcf59d23ace49a0a08cbf1504fff5e677381878e634bb7ab559c3c5
sha256(2) = 0d995f325fcf59d23ace49a0a08cbf1504fff5e677381878e634bb7ab559c3c5

run 3
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 021387b26c651595bbb66a061fb233c4f4befe91c7aeb21bb0011e1b56553603
sha256(2) = 021387b26c651595bbb66a061fb233c4f4befe91c7aeb21bb0011e1b56553603
sha256(2) = 021387b26c651595bbb66a061fb233c4f4befe91c7aeb21bb0011e1b56553603
sha256(2) = 021387b26c651595bbb66a061fb233c4f4befe91c7aeb21bb0011e1b56553603
sha256(2) = 021387b26c651595bbb66a061fb233c4f4befe91c7aeb21bb0011e1b56553603

run 4
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 5f233c66b3a0280535eb2310ddbe161f927d103f01ed9b83ffbbfd49cac7b11c
sha256(2) = 5f233c66b3a0280535eb2310ddbe161f927d103f01ed9b83ffbbfd49cac7b11c
sha256(2) = 5f233c66b3a0280535eb2310ddbe161f927d103f01ed9b83ffbbfd49cac7b11c
sha256(2) = 5f233c66b3a0280535eb2310ddbe161f927d103f01ed9b83ffbbfd49cac7b11c
sha256(2) = 5f233c66b3a0280535eb2310ddbe161f927d103f01ed9b83ffbbfd49cac7b11c

run 5
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 88e37cc74e47fefc3d80ed915a89287572c0a9e4e4e81906599e09ad56a378e2
sha256(2) = 88e37cc74e47fefc3d80ed915a89287572c0a9e4e4e81906599e09ad56a378e2
sha256(2) = 88e37cc74e47fefc3d80ed915a89287572c0a9e4e4e81906599e09ad56a378e2
sha256(2) = 88e37cc74e47fefc3d80ed915a89287572c0a9e4e4e81906599e09ad56a378e2
sha256(2) = 88e37cc74e47fefc3d80ed915a89287572c0a9e4e4e81906599e09ad56a378e2
```