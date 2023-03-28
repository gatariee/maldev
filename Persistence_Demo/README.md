## Meterpreter Persistence Using Simple VirtualAlloc

### IMPORTANT
This repo contains live malware, do not run the malware on your own machine. The payload will self-replicate onto the host's Startup folder to simulate persistence.

The shellcode was generated using msfvenom's `windows/meterpreter/reverse_tcp` payload and encoded using the `shikata_ga_nai` encoder and injected via VirtualAlloc and CreateThread.

This persistence demonstration was made for educational purposes only, I am not responsible for any damage caused by the malware. It is not considered OPSEC secure by any means and should be treated as a proof-of-concept. 

And if you're here from gryphons, hi.