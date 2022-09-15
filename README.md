# How to run p2p.c program?

First, you need to compile the program.
```
gcc p2p.c -o peer
```
If that doens't work try running with a flag to link the pthread library
```
gcc p2p.c -o peer -lpthread
```

In order to run the program, you need a connections.txt (located in the same folder as the running program), that details the connections of the node it is running on.
This file will first consist of the number of connections followed by a series of ip addresses, each seperated by a new line. <br/>
Example:
```
3
192.168.11.2
192.168.11.3
192.168.11.4
```
