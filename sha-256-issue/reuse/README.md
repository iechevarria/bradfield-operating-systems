We allocate a single array to store the hash and do not zero it out between runs.

Why is the hash value different between every single call? This implies that the previous state of the hash array matters between calls.

```bash
for i in {1..5}; do echo "run $i"; ./run.sh; echo; done;
```

output:
```
run 1
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 172686d1de7f45349b439b3c7841c76ef1ee6f6fed174cb06a7abc5c88be416d
sha256(2) = f2f3e49577f3ff75104c7a6ed8ddbecad1a266389a3035516013a113c6aadb6b
sha256(2) = 620f40d8bbb0710f7806f3e6d46555f58d0fb18bea05ddf0024a513a069b54b1
sha256(2) = 8951b25c2a85dfe1a975e7e9187f0cccd91011ffafad56c29d9a974448d858d8
sha256(2) = 986200b0587eda13f85072a56a37e389f901ea5545ab37640f51a570770d8844

run 2
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 5ce2b5b689b40def2086206f1af2a49c5ef1dc035aaeda8fff103d6d163e3096
sha256(2) = 04ba417d66ff1c21d4f427c0b2d6859ceda05abea9b9ace7754be44ac19d0f34
sha256(2) = 95d1e1bcbcd54541804cdf856809bae3da82c8919a5cb66d6ae60cf7807698b4
sha256(2) = e982c1575b59807fdba7e1c194d1c7df9863998efb6c9772091e1a47e77e16ab
sha256(2) = bebd94f6f47ce4104e8871064a9e2c77b6c9a8398de310606d24f6f5f1df0cef

run 3
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 8c86cee1b99905dda8f70c734cdd6eabd2d9aa1b252bdb7785d867e730b2297d
sha256(2) = 77cfb98a4595113585da27cdf454577f24cfd9e8fa0e6cf86e22171eef0d75d8
sha256(2) = 7f5c812f2486f79a0eaa94ac295e1a953363f1add49651720556f58924901839
sha256(2) = 9c22b021eaf687cbee5eef36fdf09e62977fcc5a951806c35cbf7cff1b28fcf6
sha256(2) = 74bf7f0aa3ee3974e9991b0c778f0f91ee751a471b105cf8634e1db584121bb1

run 4
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = 103b858359cf59aefb487879425054edea131233fb4db935622446142f6a7f90
sha256(2) = e70693acdcbbf6a26e898fe456b3454bfce030b7e2b7cc8d1592b16a49a3cd5c
sha256(2) = 9e4a1c22b5fdd3ed87ad0092512ef12f1053e1a01bc0ccda31ea748fe1147b69
sha256(2) = 02cd545f6191a7dbdd557bfc8d9f58846606fa2c0f40ddbeaa9abf0711b4fc06
sha256(2) = 31652f0936eb108f9622f70340ca744564ed297c3d9e354154f1ec67357726f0

run 5
+ clang -g3 -o a.out main.c sha-256.c
+ ./a.out
sha256(2) = b8cc40731f6c87a6761fa2502fc32b34ba87df0e9f89ab302851a0e67c313499
sha256(2) = dbae7fbf1def5ea18794f75c4595d0c1358f65fa1039ff40c4671c9724b6c826
sha256(2) = 3408cac5d4c032ab1014e612917c2329b683f762a119fb1f6d8360be44835161
sha256(2) = f79ff62564e7c23ba684faa8f90a6e73ad39b818e3db614b01107c1bab77af41
sha256(2) = 78ba78a8be2ee64de9ae5a3b3ba85beacc5d75cc4e5cfe8b6dc65647e36b6f11
```
