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

Whenever one first starts the program, they will be prompted for (a) the name of the system it is running on, and (b) the fault status of the program.

## Compiling on CentOS

Note that CentOS is really annoying and until I get a better setup a couple of additional steps might be necessary. <br/>
(a) Compile with ```-std=c99``` flag <br/>
(b) Including the ```sys/select.h``` header <br/>
(c) Turning on the firewall with ```systemctl stop firewalld``` if you find the other nodes can't connect to it but can ping it. <br/>
