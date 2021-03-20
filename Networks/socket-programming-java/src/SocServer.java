/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * @author SOUMITRI CHATTOPADHYAY
 */

public class SocServer {
    public static void main(String[] args) throws IOException {
        System.out.println("Server started");

        ServerSocket ss = new ServerSocket(9999);
        System.out.println("server waiting at port 9999");

        Socket s = ss.accept(); // if server side socket accepts any request it means it is creating an object
        System.out.println("Client connected");

        // read data from the socket
        BufferedReader br = new BufferedReader(new InputStreamReader(s.getInputStream()));
        String str = br.readLine();
        System.out.println("Server received data: "+str.substring(3,8));
    }
}
