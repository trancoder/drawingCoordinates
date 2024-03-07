import pygame
import csv
import socket

# Initialize pygame
pygame.init()

# Set up the screen dimensions
SCREEN_WIDTH = 800
SCREEN_HEIGHT = 600
screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))
pygame.display.set_caption("Whiteboard")

# Colors
WHITE = (255, 255, 255)
BLACK = (0, 0, 0)

# Set up the drawing variables
drawing = False
last_pos = None
drawn_points = []

screen.fill(WHITE)

#generate csv file 
def csv_generate():
    # Save the drawn points to a CSV file
    with open('drawn_points.csv', 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        for point in drawn_points:
            csv_writer.writerow(point)

#set up TCP

# TCP server configuration
HOST = '192.168.1.220'  # localhost
PORT = 12345

# Create a TCP socket
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
    # Bind the socket to the address and port
    server_socket.bind((HOST, PORT))
    # Listen for incoming connections
    server_socket.listen()

    print(f"Server is listening on {HOST}:{PORT}")

    # Accept incoming connection
    client_socket, client_address = server_socket.accept()
    print(f"Connection established with {client_address}")               

    # Main loop
    running = True
    while running:
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.MOUSEBUTTONDOWN:
                drawing = True
                last_pos = event.pos
            elif event.type == pygame.MOUSEMOTION:
                if drawing:
                    current_pos = event.pos
                    pygame.draw.line(screen, BLACK, last_pos, current_pos, 2)
                    last_pos = current_pos                                        
                    # Convert tuple to string
                    message = ','.join(map(str, current_pos))
                    client_socket.sendall(message.encode())
            elif event.type == pygame.MOUSEBUTTONUP:
                if (drawing):
                    drawing = False
                    


        # Update the display
        pygame.display.flip()

    pygame.quit()

    # Close the connection
    client_socket.close()    
