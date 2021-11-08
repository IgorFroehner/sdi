
/** HelloServer.java **/

import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;

import java.util.List;
import java.util.ArrayList;
import java.lang.*;
import java.io.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;
import java.time.*;
import java.util.stream.Collectors;

public class ChatServer implements Chat {
    private static String messagesFolder = "server_messages";
    private static List<String> messagesNameList;

    public static void main(String[] args) {
        try {
            Path path = Paths.get(messagesFolder);
            if (!Files.exists(path)) {
                Files.createDirectories(path);                
            }

            messagesNameList = Files.list(path)
                .map(Path::getFileName)
                .map(Path::toString)
                .filter(f -> f.endsWith(".serv"))
                .collect(Collectors.toList());

            // Instancia o objeto servidor e a sua stub
            ChatServer server = new ChatServer();
            Chat chat = (Chat) UnicastRemoteObject.exportObject(server, 0);

            // Registra a stub no RMI Registry para que ela seja obtAida pelos clientes
            Registry registry = LocateRegistry.createRegistry(6600);

            registry.bind("Chat", chat);
            System.out.println("Servidor pronto");
        } catch (IOException e) {
            System.err.println("Failed while creating the directory");
            System.exit(1);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public void send(String user, List<String> message) throws RemoteException {
        try {
            System.out.println("[" + LocalTime.now() + "] Message Received from " + user);

            String messageName = user + "-" + messagesNameList.size() + ".serv";
            Path path = Paths.get(messagesFolder + "/" + messageName);
            Files.write(path, message, StandardCharsets.UTF_8, StandardOpenOption.CREATE);

            messagesNameList.add(messageName);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public List<String> receive(int messageId) throws RemoteException {
        if (messageId >= messagesNameList.size()) return null;
        try {
            Path path = Paths.get(messagesFolder + "/" + messagesNameList.get(messageId));
            return Files.readAllLines(path);
        } catch (IOException e) {
            e.printStackTrace();
        }     
        return null;
    }

    public String receiveFileName(int messageId) throws RemoteException {
        if (messageId >= messagesNameList.size()) return null;
        return messagesNameList.get(messageId);
    }

}
