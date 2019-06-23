The engine's network system consists of the following components:

* INetwork - provides the low-level networking functionality;
* INetChannel - represents the network connection channel (sv<->cl); all network messages are passing through the channel;
* INetServer - network channel that provides the server-side network functionality (one-to-many interaction with the other players);
* INetClient - network channel that provides the client-side network functionality (one-to-one interaction with the server this client is connected to (if present));
* INetAdr - network address representation;
* INetMsg - network message component; contains message type and data to pass via the network channel;