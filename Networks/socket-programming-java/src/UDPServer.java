/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;

/**
 * @author SOUMITRI CHATTOPADHYAY
 */

public class UDPServer {
    public static void main(String[] args) throws Exception {

        final int PORT = 9999;
        DatagramSocket ds = new DatagramSocket(PORT);

        byte[] b1 = new byte[1024];
        DatagramPacket dp = new DatagramPacket(b1,b1.length);
        ds.receive(dp);

        String str = new String(dp.getData(),0,dp.getLength());
        System.out.println("S: str = "+str.trim());
        int num = Integer.parseInt(str.trim());
        num = num*num;

        InetAddress ia = InetAddress.getLocalHost();
        byte[] b2 = String.valueOf(num).getBytes();
        DatagramPacket dp1 = new DatagramPacket(b2,b2.length,ia,dp.getPort());
        ds.send(dp1);
    }
}
