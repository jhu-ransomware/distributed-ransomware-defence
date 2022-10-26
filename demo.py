from re import I
import socket
import sys

PORT = 10100
NUM_NODES = 3

INT_SIZE_C = 4

# Create a TCP/IP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_address = ('', PORT)
print(f"starting up on {server_address} port {PORT}")
sock.bind(server_address)
sock.listen(NUM_NODES)

while True:
    print('waiting for a connection')
    connection, client_address = sock.accept()

    try:
        print(f"connection from {client_address}")

        # Receive the data in small chunks and retransmit it
        while True:
            data = connection.recv(1024)
            node = int.from_bytes(data[:4], byteorder=sys.byteorder)
            diagnosis = [x for x in data[4:]]
            
            print(f"Node {node} diagnosis:\n{data[4:]}")
            break

    finally:
        # Clean up the connection
        connection.close()
