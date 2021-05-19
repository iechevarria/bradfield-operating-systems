# reproducing some weird behavior

I’m seeing some weird behavior with `calc_sha_256` - it looks like it’s sensitive to the state of the input `hash`.

I tested this by doing the following:
1. set `hash` to be all 0s in the loop before each `calc_sha_256` call
2. call again once outside the loop without setting `hash` to all 0s
3. call again after setting `hash` to all 0s again

The result is that the first and last hashes match, but not the middle one.

```
Found summand to be 1159581712, since sha256(53189102 + 1159581712 = 1212770814) = 000030c86a42a9510ca8c6c98ad00af7bdcfd155b88b4c1355df36752d2c4de3
Found summand to be 1159581712, since sha256(53189102 + 1159581712 = 1212770814) = 4282f7695b91d16968c3956c82befecdf6b809d45ec6343e8e5b56a507c730f7
Found summand to be 1159581712, since sha256(53189102 + 1159581712 = 1212770814) = 000030c86a42a9510ca8c6c98ad00af7bdcfd155b88b4c1355df36752d2c4de3
```

This code reproduces the weird behavior I'm seeing. Run `./repro.sh` to see it in action.
