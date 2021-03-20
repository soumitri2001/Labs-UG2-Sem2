/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.net.*;

/**
 * @author SOUMITRI CHATTOPADHYAY
 */

public class UDPClient {
    public static void main(String[] args) throws Exception {
        int i = 8;
        byte[] bytes = String.valueOf(i).getBytes(); // getBytes() belong to String class
        InetAddress ia = InetAddress.getLocalHost();
        final int PORT = 9999;

        DatagramSocket ds = new DatagramSocket();
        DatagramPacket dp = new DatagramPacket(bytes,bytes.length,ia,PORT);
        ds.send(dp);

        byte[] b1 = new byte[1024];
        DatagramPacket dp1 = new DatagramPacket(b1,b1.length);
        ds.receive(dp1);

        String str = new String(dp1.getData(),0,dp1.getLength());
        System.out.println("result is "+str.trim());
    }
}
