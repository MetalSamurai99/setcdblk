# setcdblk

A tool to reset the blobk size for non Sun CD ROM drives on Solaris.

Solaris expects CD ROM drives to have a block size of 512 bytes, but most use a block size of 2048 bytes. This means you can't boot a Sun workstation from many third party optical drives and you can't even read data from them. This tool fixes the second problem, allowing you to read data from a third party drive *after* booting.

