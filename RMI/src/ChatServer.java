
/** HelloServer.java **/

import java.rmi.*;
import java.rmi.server.*;
import java.rmi.registry.*;

import java.util.List;
import java.util.ArrayList;
import java.io.*;
import java.nio.file.*;
import java.nio.charset.StandardCharsets;

public class ChatServer implements Chat {
    private int messageCount = 0;
    private String messagesFolder = "server_messages";

    public static void main(String[] args) {
        try {
            // Instancia o objeto servidor e a sua stub
            ChatServer server = new ChatServer();
            Chat chat = (Chat) UnicastRemoteObject.exportObject(server, 0);

            // Registra a stub no RMI Registry para que ela seja obtAida pelos clientes
            Registry registry = LocateRegistry.createRegistry(6600);

            registry.bind("Chat", chat);
            System.out.println("Servidor pronto");
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    public void send(String user, List<String> message) throws RemoteException {
        try {
            Path path = Paths.get(messagesFolder + "/" + user + "-" + (messageCount++) + ".serv");
            Files.write(path, message, StandardCharsets.UTF_8, StandardOpenOption.CREATE);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public int receive() throws RemoteException {
        return 1;
    }

}
