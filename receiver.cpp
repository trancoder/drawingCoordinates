#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <gtkmm.h> // to plot data
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <cstdlib> // this is for std::system

// Define a structure to represent coordinates
struct Coordinate {
    double x;
    double y;
}; 

// Global variables for data sharing between threads
std::mutex mtx;
std::queue<Coordinate> coordinateQueue;
std::condition_variable cv;

// Function to plot data
void plotData() {
    // Initialize Gtkmm application
    auto app = Gtk::Application::create();

    // Create Gtkmm window and widgets
    Gtk::Window window;
    Gtk::DrawingArea drawingArea;
    window.add(drawingArea);      

    // Connect signal_draw() signal with a lambda function for drawing    
    drawingArea.signal_draw().connect([&drawingArea](const Cairo::RefPtr<Cairo::Context>& cr) {
        
        // Print a statement before setting the color
        std::cout << "Processing the coordinates " << std::endl;

        // Lock the mutex before accessing the shared data
        std::unique_lock<std::mutex> lock(mtx);
        
        // Draw all coordinates in the queue
        while (!coordinateQueue.empty()) {
            const Coordinate& coord = coordinateQueue.front();
            cr->set_source_rgb(0.0, 0.0, 0.0);
            cr->arc(coord.x, coord.y, 3.0, 0.0, 2.0 * M_PI);
            cr->fill();
            coordinateQueue.pop();
        }
        return true; // Return true to indicate drawing is complete
    });
    // Show the window
    window.show_all();

    app->run(window);  
}


int main() {

    // Create a thread for Gtkmm to plot the data
    std::thread plotThread(plotData);

    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Connect to server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Port number
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.220"); // IP address of the server

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(client_socket);
        return -1;
    }
    
    // receive data
    char buffer[4096];
    std::string input;
    while (true) {        
        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Connection closed" << std::endl;
            break;
        }
        std::cout << "Server: " << buffer << std::endl;

        //Parse received data (assuming format: "x1 y1;x2 y2;x3 y3;...")
        std::stringstream ss(buffer); // Create a stringstream with the input buffer

        double x, y;
        char comma; // To store the comma character
        ss >> x >> comma >> y; // Extract values separated by a comma

        if (comma == ',') { // Check if the comma was successfully extracted
            // Values were successfully parsed
            std::cout << "Parsed values: x = " << x << ", y = " << y << std::endl;
        } 

        {
            std::lock_guard<std::mutex> lock(mtx);
            coordinateQueue.push({ x, y});
        }
        cv.notify_one(); // Notify the plotting thread that new data is available
    }    

    // Close socket
    close(client_socket);

    // Join the plotThread to ensure it finishes before exiting main
    plotThread.join();

    // Print a statement before setting the color
    std::cout << "PlotThread Joined " << std::endl;

    return 0;
}
