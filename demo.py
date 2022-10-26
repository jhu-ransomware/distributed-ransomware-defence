from re import I
import socket
import sys

PORT = 10100
NUM_NODES = 2

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
            diagnosis = []
            for i in range(4, 4 + 4  * NUM_NODES, 4):
                print("Here")
                diagnosis.append(int.from_bytes(data[i:i+4], byteorder=sys.byteorder))
            
            print(f"Node {node} diagnosis:\n{diagnosis[4:]}")
            break

    finally:
        # Clean up the connection
        connection.close()
