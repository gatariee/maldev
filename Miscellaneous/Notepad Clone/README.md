## Notepad Clone :sleep:

### Note

Shellcode starts beacon that points to a (probably?) live C2 server self-hosted by myself, please don't run it.

### ?
- starts a new notepad process (only works on <win11, update process launch accordingly for win11)
- injects shellcode into new notepad proces, injection is done via `CreateRemoteThread`