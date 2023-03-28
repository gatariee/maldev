## Clownjection via Clownpad Impersonation

### IMPORTANT
This repo contains live malware, do not run the malware on your own machine. The payload will self-replicate onto the host's Startup folder to simulate persistence.

The shellcode was generated using msfvenom `windows/meterpreter/reverse_tcp` payload and encoded using the `shikata_ga_nai` encoder(x86/x32). 

The binary launches a new instance of `notepad.exe` (which shows up as a valid process), and injects the shellcode into the process using VirtualAllocEx and CreateRemoteThread. 

This persistence demonstration was made for educational purposes only, I am not responsible for any damage caused by the malware. It is not considered OPSEC secure by any means and should be treated as a proof-of-concept. :clown:

### :clown:
As a proud clown, I've seen some weird stuff in my time. But stumbling upon a room filled with skulls? That was a new one. At first, I was spooked, thinking maybe I had accidentally wandered into a haunted house. But as I took a closer look, I recognized them for what they were – clown skulls. Apparently, it's a tradition among us clowns to collect the skulls of our fallen friends and display them as a way of honoring their memory.

As I was looking at the skulls, I noticed a strange device on the wall. Upon further investigation, I realized it was a high-tech notepad, capable of "process injection" and "impersonation." I couldn't believe my clown eyes – what was a notepad doing in a room filled with skulls? It didn't make any sense.

Curiosity getting the better of me, I picked up the notepad and began exploring its features. It was then that I stumbled upon a file containing the secret clown code – a way for clowns to communicate with one another in code, keeping our secrets safe from outsiders. And it was all thanks to the power of "process injection" and "impersonation" that the notepad provided.

As I continued to investigate the notepad, I realized that it had been left behind by a fellow clown who had since passed away. Looking at the skulls on the wall, I realized that this notepad was likely his way of leaving a legacy – a way of passing on his knowledge to future generations of clowns. It was a poignant moment, and one that made me realize just how important it is for us clowns to stick together and support one another.
