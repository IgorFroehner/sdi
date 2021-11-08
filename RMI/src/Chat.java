import java.rmi.*;
import java.io.*;
import java.util.*;

public interface Chat extends Remote {
   public void send(String user, List<String> message) throws RemoteException;
   public List<String> receive(int messageId) throws RemoteException;
   public String receiveFileName(int messageId) throws RemoteException;
}
