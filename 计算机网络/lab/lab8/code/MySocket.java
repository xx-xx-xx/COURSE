import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

public class MySocket {
    private static boolean exit = false;
    private static ServerSocket server;
    private static final ExecutorService cachePool = Executors.newCachedThreadPool();
    private static final String localPath = System.getProperty("user.dir")+"/assets";
    public static void main(String[] args) throws IOException {
        Socket client;
        server = new ServerSocket(5507);
        Runtime.getRuntime().addShutdownHook(new Exit()); //register exit handler hook
        LogPrinter.setLogState(true); //set log printer on
        while (!exit) {
            try {
                client = server.accept();
                if (client != null && client.isConnected()) {
                    //Get a new request, handle it using cached thread pool
                    cachePool.execute(new ClientConnection(client, localPath));
                    //Print the state of thread pool to the screen
                    LogPrinter.threadPoolInfo((ThreadPoolExecutor)cachePool);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }
    //print the state of thread pool when ctrl-c
    private static class Exit extends Thread{
        @Override
        public void run() {
            super.run();
            exit = true;
            cachePool.shutdown(); //wait util all threads terminated
            try {
                server.close(); //close server
            } catch (IOException e) {
                e.printStackTrace();
            }
            LogPrinter.threadPoolInfo((ThreadPoolExecutor)cachePool);
        }
    }
}