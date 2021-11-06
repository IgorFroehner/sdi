import java.rmi.*;
import java.io.*;
import java.util.*;

public interface Chat extends Remote {
   public void send(String user, List<String> message) throws RemoteException;
   public int receive() throws RemoteException;
}
