import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.ServerSocket;

public class Q31_SocketProg {

    public static final int PORT = 9999;
    public static final String IP = "localhost";

    // class for client socket
    static class SocketClient {
        void runClient() throws Exception {

            Socket s = new Socket(IP, PORT);
            String str = "Studying";

            // print data from output port of the socket
            OutputStreamWriter os = new OutputStreamWriter(s.getOutputStream());
            PrintWriter out = new PrintWriter(os);
            out.println(str); // sending the data to server
            os.flush();
            System.out.println("Data sent from client: " + str);
        }
    }

    // class for server socket
    static class SocketServer {
        void runServer() throws Exception {
            System.out.println("Server started");

            ServerSocket ss = new ServerSocket(PORT);
            System.out.println("server waiting at port " + PORT);

            Socket s = ss.accept(); // if server side socket accepts any request it means it is creating an object
            System.out.println("Client connected");

            // read data from the socket
            BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
            String str = br.readLine();
            System.out.println("Server received data: " + str.substring(3, 8));
        }
    }

    public static void main(String[] args) {
        SocketServer server = new SocketServer();
        try {
            server.runServer();
        } catch (Exception e) {
            e.printStackTrace();
        }

        SocketClient client = new SocketClient();
        try {
            client.runClient();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}