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
(b) Turning on the firewall with ```systemctl stop firewalld``` if you find the other nodes can't connect to it but can ping it. <br/>

## Running on Windows
Because the library is written using unix libararies, requires some hacks to be able to run on Windows. <br/>
1) Open up cygwin.bat shortcut on the desktop <br/>
2) Navigate to /cygdrive/c/Users/OWL/Desktop/GroundStationWork/Distributed Defense System/DistributedDefenseSystem. From here, you can run and compile as normal. <br/>
3) Also, will likely need to turn off windows firewall for the network that all the devices are on. You can do this by going to "windows defense firewall with advanced security", clicking on "windows defender firewall properties" in the center pane, clicking on "window defender firewall properties", then going to public profile and click on protected networkc connection and then uncheck "ethernet" (this is the network everything is on). 

## TODOS ON IMPLEMENTATION

I'm going to keep a running list of todos on here with the purposes of me not forgetting about them. These are things that don't necessarily keep the implementation from working but will need to be improved. <br/>
1) Add a mutex lock around the tested up array usage.
2) Better way of keeping track of how big to make array.
3) Don't requiring entering your own node number
4) Deal with case where all connected nodes are failed.
5) Ensure goes through an adaptive round with new new status before updating

## Notes on detection module
The detection module is purposely designed to be swapable. Currently, it uses a file entrophy based approach to do so. If this would ever want to be changed, the only code that would be affected is the "run_detection" function in monitor.c (as well as it's called functions) to instead use a different approach. Also, note that the code building the lookup table used by "run_detection" would no longer be needed either. 
