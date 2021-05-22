# sha 256 weirdness

This directory contains three different scenarios that demonstrate unexpected behavior when calling `calc_sha_256`.

This was motivated by the fact that I was unable to validate any of the hashes that worker threads computed in the mini miner lab.

Huge thanks to [Geoffrey](https://github.com/ggilmore) for investigating with me and for teaching me some best practices along the way.

## update: solved

The third argument for `calc_sha_256` is bytes, not bits, and the call to this function was `calc_sha_256(hash, (const void *)(&sum), 32);` where `sum` was a 32 bit integer. It works fine when called as follows: `calc_sha_256(hash, (const void *)(&sum), 4);`.
