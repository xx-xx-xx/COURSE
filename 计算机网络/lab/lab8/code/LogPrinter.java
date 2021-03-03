import java.util.concurrent.ThreadPoolExecutor;

public class LogPrinter {
    private static boolean logState = false;
    public static void threadInfo(String method, String file){
        if (logState){
            String name = Thread.currentThread().getName();
            System.out.println("[" + method + " : " + file + "] : " + name + " is running now");
        }
    }
    public static void threadPoolInfo(ThreadPoolExecutor pool){
        if (logState){
            if (pool.isShutdown()){
                System.out.println("Thread pool is shut down");
            } else if (pool.isTerminating()) {
                System.out.println("Thread pool is terminating");
            } else if (pool.isTerminated()) {
                System.out.println("Thread pool is terminated");
            } else {
                System.out.println("Thread pool is active");
            }
            System.out.println("There are " + pool.getActiveCount() + " active threads in the pool");
        }
    }
    public static boolean getLogState(){
        return logState;
    }
    public static void setLogState(boolean state){
        logState = state;
    }
}
