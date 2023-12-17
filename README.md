# SMQ
Simple Message Queue created for the purpose of learning about tcp sockets and message queues.
## Description and Usage
TODO
## Running
The server can be run using docker:
```bash
docker run ghcr.io/mati2251/smq-server:latest -p 3000:3000 --rm
```
The client example can be run using docker too:
To run the publisher
```bash
docker run --rm ghcr.io/mati2251/smq-client:latest pub
```
To run the subscriber
```bash
docker run --rm ghcr.io/mati2251/smq-client:latest sub
```
