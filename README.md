c1000k
======

This is the TCP server-client suit to help you test if your OS supports c1000k(1 million connections).

## Usage

### Download and compile

	wget --no-check-certificate https://github.com/ideawu/c1000k/archive/master.zip
	unzip master.zip
	cd c1000k-master
	make

### start the server, listen on port 7000~7099

	./server 7000

### run the client

	./client 127.0.0.1 7000

The server will ouput message like this and quit:

	connections: 153
	error: Too many open files

It says the server can only accept 153 connections, it reaches the max-open-files limitation so it quit.

The client will output message like this:

	connections: 154
	error: Connection refused

