import socket
import sys
import pygame, sys
from pygame.locals import *

NUM_NODES = 3
results = [0 for _ in range(NUM_NODES)]

SCREEN_WIDTH = 500
SCREEN_HEIGHT = 300

MAX_BOX_IN_ROW = 6

def convert_network_bytes_to_int(bytes):
    return socket.ntohl(int.from_bytes(bytes, byteorder=sys.byteorder))

def draw_rect(display):
    WHITE=(255,255,255)
    GREY = (230, 230, 230)
    display.fill(GREY)

    GREEN = (0,255,0)
    RED = (255,0,0)
    BLACK = (0,0,0)

    GAP = 20
    SIZE_RECT = 50

    # Draw title
    font = pygame.font.Font(None, 40)
    text = font.render("Fault Status", True, BLACK)
    text_rect = text.get_rect(center=(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 7))
    display.blit(text, text_rect)

    # Add key
    font = pygame.font.Font(None, 25)
    text = font.render("Faulty", True, BLACK)
    pygame.draw.rect(display, RED, (3, SCREEN_HEIGHT - SCREEN_HEIGHT / 12, 15, 15))
    text_rect = text.get_rect(topleft = (22, SCREEN_HEIGHT - SCREEN_HEIGHT / 12))
    display.blit(text, text_rect)

    font = pygame.font.Font(None, 25)
    text = font.render("Non-Faulty", True, BLACK)
    pygame.draw.rect(display, GREEN, (3, SCREEN_HEIGHT - SCREEN_HEIGHT / 6, 15, 15))
    text_rect = text.get_rect(topleft = (22, SCREEN_HEIGHT - SCREEN_HEIGHT / 6))
    display.blit(text, text_rect)

    # Calculate offsets for the rectangles
    calc_row_len = NUM_NODES if NUM_NODES < MAX_BOX_IN_ROW else MAX_BOX_IN_ROW
    rect_row_len = (SIZE_RECT * calc_row_len) + GAP * (calc_row_len - 1)
    offset = (SCREEN_WIDTH - rect_row_len) / 2

    font = pygame.font.Font(None, 40)

    for i in range(0, NUM_NODES):
        if i == MAX_BOX_IN_ROW:
            offset = (SCREEN_WIDTH - rect_row_len) / 2

        color = RED if results[i] else GREEN
        offset += (GAP + SIZE_RECT) if i != 0 and i != MAX_BOX_IN_ROW else 0
        text = font.render(f"{i}", True, WHITE)

        off_center = (-SIZE_RECT - GAP / 2) if i < 6 else GAP / 2

        rect = pygame.draw.rect(display,color,(offset, SCREEN_HEIGHT / 2 + off_center,SIZE_RECT,SIZE_RECT))
        display.blit(text, rect)

def draw(sock):
    pygame.init()

    DISPLAY=pygame.display.set_mode((SCREEN_WIDTH,SCREEN_HEIGHT),0,32)

    draw_rect(DISPLAY)

    while True:
        pygame.display.update()
        for event in pygame.event.get():
            if event.type==QUIT:
                pygame.quit()
                sys.exit()
        try:
            sock.settimeout(2.0)
            connection, client_address = sock.accept()

            try:
                # Receive the data in small chunks and retransmit it
                while True:
                    data = connection.recv(1024)
                    for i in range(NUM_NODES):
                        results[i] = convert_network_bytes_to_int(data[i * 4:(i+1) * 4])

                    break

            finally:
                # Clean up the connection
                connection.close()
        except socket.timeout:
          continue
        
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
