import socket
import sys
import pygame, sys
from pygame.locals import *

NUM_NODES = 2
results = [0 for element in range(10)]

def draw_rect(display):
    WHITE=(255,255,255)
    display.fill(WHITE)

    GREEN = (0,255,0)
    RED = (255,0,0)

    GAP = 10
    SIZE_RECT = 50

    for i in range(0, NUM_NODES):
        color = RED if results[i] else GREEN
        offset = (GAP + SIZE_RECT) * i
        pygame.draw.rect(display,color,(offset + SIZE_RECT,(400 - SIZE_RECT) / 2,SIZE_RECT,SIZE_RECT))

def draw(sock):
    pygame.init()

    DISPLAY=pygame.display.set_mode((500,400),0,32)

    draw_rect(DISPLAY)

    while True:
        pygame.display.update()
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()

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
        
        draw_rect(DISPLAY)

def main():
    PORT = 10200

    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

    server_address = ('', PORT)
    print(f"starting up on {server_address} port {PORT}")
    sock.bind(server_address)
    sock.listen(NUM_NODES)

    draw(sock)

if __name__ == "__main__":
    main()