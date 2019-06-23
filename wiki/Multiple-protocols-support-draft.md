* IProtocol // all protocols implementations should be derived from this
* CProtocolProxy/CProtocolDetector // class that will (detect and) send the messages via the right protocol implementation (it will hold it inside)

CProtocolProxy::SendStuffText("blah") -> IProtocol *pCurrentProtocol -> SendStuffText("blah") -> implementation-specific details

NOTE: The drawback of this system is that each protocol should contain all potentially possible entries (if one protocol is able to do thing "A" and the other one is able to do thing "B" then IProtocol should contain both "A" and "B" and all implementations must also contain them with null or actual implementation