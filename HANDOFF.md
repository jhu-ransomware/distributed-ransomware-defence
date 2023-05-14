# Note about this document
I started this just to put some general thoughts and tricks about the program and things that will help ease the transition. This is meant to complement, not replace the README document.
I started this because I thought it'd be easier to start from a blank document. 

# Notes on the README
The most helpful part of the README is going to be the  **How to run** section and the **Running on Windowns** section. Other sections are still worth reading. The
implementation TODOs were just things I thought of while implementing over the year and may or may not be relevant. I had just wanted to track them when I thought of them. <br/>  

# General Notes about running the program

1) In ```constants.h```, there is a constant called ```NUM_NODES```, this needs to be changed depending on the number of nodes, otherwise unexpected behavior will result. <br>
2) Depending on the system used, different flags might need to be added to the Makefile. If you ever run into an issue of a library not being found, try adding the flag for it first <br/>
3) When running on the ugrad systems, there was an issue with the Math library not having ```log2``` function. I added an implementation of it but it's commented out for other systems. If you run into an issue with ```log2``` not existing, try uncommenting that. <br/>
4) Generally, when running the program, you will be promopted for 3 things: 1 - Node number of the host computer. Node number is 0-indexed and should be consistent across different systems. I.e. the node number put in, should be the same as the number for that node in the ```connections.txt``` file on other nodes.
2 - Fault status. Generally, you will want to set this to 0 for not faulty, but if you want to test a faulty node without running ransomware you can manually make a node faulty. 
3 - Whether or not to send to a demo. I wrote a simple a python demo app that you can send the results to if you run ```demo.py``` and set this to 1. If not running the 
python script set to 0. <br/>
6) This brings me to my next point, the ```demo.py```. This was a super rough simple UI I wrote for demonstration. Feel free to expand it, use it, or simple get rid of it.
Because I was just trying to get a simple app going, it communicates via sending the results to the python app using sockets, but there is a definitely a better way to do it. <br/>
7) Other note on the UI, is that when the program is running, it prints out the results of the "tested_up" array before and after each testing round. You can then press 2 to see the current
diagnosis of the entire system.
