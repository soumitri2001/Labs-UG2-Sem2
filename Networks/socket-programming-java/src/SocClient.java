/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintStream;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * @author SOUMITRI CHATTOPADHYAY
 */

public class SocClient {
    public static void main(String[] args) throws IOException {
        String ip = "localhost";
        int port = 9999; // 0 to 65535, 0-1023 are reserved

        Socket s = new Socket(ip,port);
        String str = "Studying";

        // print data from output port of the socket
        OutputStreamWriter os = new OutputStreamWriter(s.getOutputStream());
        PrintWriter out = new PrintWriter(os);
        out.println(str);  // sending the data to server
        os.flush();
        System.out.println("Data sent from client: "+str);
    }
}
