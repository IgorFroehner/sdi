/** HelloClient.java **/

import java.rmi.registry.*;

import java.io.*;
import java.lang.*;
import java.util.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;

public class ChatClient {
    private static Chat chat;
    private static String user;
   
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("ChatClient <host> <user>");
        }
        String host = args[0];
        user = args[1];

        try {

            // Obtém uma referência para o registro do RMI
            Registry registry = LocateRegistry.getRegistry(host, 6600);

            // Obtém a stub do servidor
            chat = (Chat) registry.lookup("Chat");

            new Thread(sender).start();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static Runnable sender = new Runnable() {
        public void run() {
            Scanner sc = new Scanner(System.in);

            while (sc.hasNext()) {
                String fileName = sc.nextLine();
            
                try {

                    List<String> lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);

                    chat.send(user, lines);
                } catch (Exception re) {
                    re.printStackTrace();
                }
            
            }
        }
    };
    
    // private static Runnable receiver = new Runnable() {
    //     public void run() {
    //         Scanner sc = new Scanner(System.in);

    //         while (sc.hasNext()) {
                    
    //             String fileName = sc.nextLine();
            
    //             try {
    //                 File file = new File(fileName);
    //                 chat.send(file);
    //             } catch (Exception re) {
    //                 re.printStackTrace();
    //             }
            
    //         }
    //     }
    // };

}

