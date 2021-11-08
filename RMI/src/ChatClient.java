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
   
    private static String messagesFolder;

    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println("ChatClient <host> <user>");
        }
        String host = args[0];
        user = args[1];

        messagesFolder = "./" + user + "_messages";
        Path path = Paths.get(messagesFolder);
        if (!Files.exists(path)) {
            try {
                Files.createDirectories(path);
            } catch (IOException e) {
                System.err.println("Failed while creating the directory");
                System.exit(1);
            }
        }

        try {

            // Obtém uma referência para o registro do RMI
            Registry registry = LocateRegistry.getRegistry(host, 6600);

            // Obtém a stub do servidor
            chat = (Chat) registry.lookup("Chat");

            new Thread(sender).start();
            new Thread(receiver).start();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    private static Runnable sender = new Runnable() {
        public void run() {
            Scanner sc = new Scanner(System.in);

            System.out.println("Input the name of the file to send:");

            while (sc.hasNext()) {
                String fileName = sc.nextLine();
            
                try {
                    List<String> lines = Files.readAllLines(Paths.get(fileName), StandardCharsets.UTF_8);

                    chat.send(user, lines);
                } catch (NoSuchFileException e) {
                    System.err.println("The file " + fileName + " does not exists");
                } catch (Exception re) {
                    re.printStackTrace();
                }
            
            }
        }
    };
    
    private static Runnable receiver = new Runnable() {
        public void run() {
            int messageCounter = 0;
            
            try {
                while (true) {
                    
                    while (true) {
                        List<String> message = chat.receive(messageCounter);
                        if (message == null) break;
                        String messageName = chat.receiveFileName(messageCounter);

                        Path path = Paths.get(messagesFolder + "/" + messageName);
                        Files.write(path, message, StandardCharsets.UTF_8, StandardOpenOption.CREATE);

                        System.out.println("File " + messageName + " received");
                        messageCounter++;
                    }
                
                    Thread.sleep(1000);
                }

            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    };

}

