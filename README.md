# Spanning-Tree-Protocol
Here are the details of my algorithm and its implementation.

I have a struct named bridge (struct has all basic functionalities of a class and it has no problem of public and private data members). Its main data members are its root port (what it considers to be), distance from the root port (that it considers), itself (its own name i.e id), its parent (the bridge just above it in the path to root port). The initial three data members form the bridge configuration message. It has a member function comp_and_updt() declared (defined in bridge.cpp file) which takes a message from a lan segment and the lan and updates (if possible) the message on a bridge.

__________
Algorithm:
__________
1st part:

Initially all bridges are initialised with their config messages. I make a lan_map which is a mapping of each lan segment to the bridges it is connected to. I consider each lan segment as a vector of messages i.e. its content is made up of the messages present on a bridge at a given instance (made my iterating over lan_map). Now the message on a bridge needs to update so I iterate over each bridge and call the comp_and_updt() function. The comp_and_updt function contains the various conditions according to which the message needs to change. Like for e.g. If the root ID of a lan message (i.e. message it gets from the various bridges connected to that lan) is better than its own, then it makes that as its RP (root port) (changes other ports to DP) and finds its suitable parent from the message.
If the root ID's are equal then we see the distance. If the distance in the lan message is one less then we have three conditions. 1) If the parent of the bridge is bad (lower in ID rank) than the bridge ID on the message then we change the root port to that. 2) If the parent of the bridge is equal than the bridge ID on the lan message and the link/port in consideration is not the root port, then we deactivate it. 3) If the parent of the bridge is better than the bridge ID on the lan message and the link/port in consideration is not the root port, then also we deactivate it.
If the root ID's as well as the distances but the ID's are not equal then if our bridge ID is bad and the link is not the root port then we deactivate it.

Like this at the end we will be be left with only one message on the lan segments (due to some port getting changed to RP and some to NP). When this happens we know that Spanning tree protocol has finished and the tree has been formed.


2nd part:

For this forwarding part, I maintain two maps lan_to_bridge and bridge_to_lan which are iterated through alternately. Initially the lan segment on which the sender is there fills the lan_to_bridge on all the bridges connected to it. At this time the forwarding tables are updated. Now we iterate through the lan_to_bridge and populate the bridge_to_lan map (look at the forwarding tables if entries are there). We clear the lan_to_bridge map now. Now again we fill the lan_to_bridge map by iterating through the bridge_to_lan map. So we see how the packet is forwarded from one bridge to another with a lan segment in between. Finally lan_to_bridge will contain no entry as the packet would have reached it destination. This is where we stop.

____________________________________
Regarding the tracing of my program:
____________________________________

For the first half of the program, the trace results are shown for each port of a bridge. So messages will be repeated. So whenever the lan vectors are filled, I go to each bridge and see the messages present on each of the lan segment connected to it and decide suitably whether that message was sent or recieved by that bridge (by seeing if it was a RP or DP).
For the 2nd part of the assignment, the sender LAN address and the receiver LAN address remains the same everytime (this is my assumption, i don't know if the problem statement meant the intermediate LANs). At each time some bridges will be receiving and some will be sending. It is captured in the trace whenever the lan_to_bridge map and the bridge_to_lan map is filled.
