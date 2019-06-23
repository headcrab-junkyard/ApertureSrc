# New network protocol version

Draft notes for engine protocol 49  
(This version won't be compatible with the 48 and doesn't meant to be)

### Features:
* Dynamic resource limits and additional data that server send to client on connection
* Resources handling can be rewritten to use dynamic-sized arrays and just allocate new resource when server says without any limit (is it good?)
* svc_resourcelimit? (short for type and byte for value?)
* Fix the way of work for svc_resourcelist (it sends all resource names but size of msg is limited to 64KB)