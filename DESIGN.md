1. Then main tradeoff between UTF-32 and UTF-8 is memory efficiency .vs how easy to access randomly. 
For UTF-32, because it is fixed with 4 bytes always, UTF-32 uses a lot of memory to store the data, 
while UFT-8 uses fewer bytes for thoes who can be represented as fewer bytes. 
For example 'A' in UTF-32 : 0x00000041,
            'A' in UFT-8 0x41.

2.  The leading 10 on continuation bytes in UTF-8 could make user easier to dectect error. The continuation bytes are clearly different with first bytes characters, which allows software to know if there is an error while encoding.
    Without this restriction, software is hard to know which give byte is a start byte or an end byte. This would make users much harder to dectect errors and resolve the problem decoding the data correctly. To sum up, leading 10 on continuation bytes ensures to distinguish both users and software for proper synchronization.