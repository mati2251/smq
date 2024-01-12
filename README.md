# SMQ
Simple Message Queue created for the purpose of learning about tcp sockets and message queues. 
## Running
The server can be run using docker:
```bash
docker  -p 3000:3000 --rm run ghcr.io/mati2251/smq-server:latest 
```
The client example can be run using docker too:
```bash
docker run --rm ghcr.io/mati2251/smq-client:latest  
```
# License
MIT