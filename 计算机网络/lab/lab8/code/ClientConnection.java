import java.io.*;
import java.net.Socket;
import java.text.ParseException;
import java.util.Map;

public class ClientConnection extends Thread{
    private InputStream requestStream;
    private OutputStream responseStream;
    private final String serverRoot;
    private String fileName;
    private static final int MAX_BUFFER_LENGTH = 8192;
    private static final String account = "3180105507";
    private static final String password = "5507";
    private final byte[] responseBuffer = new byte[MAX_BUFFER_LENGTH];
    ClientConnection(Socket client, String deployPath) {
        serverRoot = deployPath; //address convert
        try{
            requestStream = client.getInputStream();
            responseStream = client.getOutputStream();
        } catch (Exception e){
            e.printStackTrace();
        }
    }

    @Override
    public void run() {
        super.run();
        try {
            RequestParser request = new RequestParser(requestStream);
            fileName = request.getFilePath();

            if (request.getMethod().equals("GET")){
                LogPrinter.threadInfo("GET",request.getSource());
                handleGET(request);
            }
            if (request.getMethod().equals("POST")){
                LogPrinter.threadInfo("POST",request.getSource());
                handlePOST(request);
            }
            //ico ignored
            if (request.getFileType().equals("ico")) return;
            
        } catch (IOException | ParseException e) {
            e.printStackTrace();
        }
    }

    private void handlePOST(RequestParser request) throws IOException {
        String src = request.getSource();
        src = src.substring(src.lastIndexOf('/')+1);
        if (src.equals("dopost")){ //200
            Map<String, String> reqMap = request.getRequestMap();
            String login = reqMap.get("login");
            String pass = reqMap.get("pass");
            String msg;
            if (login.equals(account) && pass.equals(password)){
                msg = "Login Success!";
            } else {
                msg = "Login Failed.";
            }
            msg = "<html><body>" + msg + "</body></html>";
            setResponseHeaders(200,msg.getBytes().length,request);
            responseStream.write(msg.getBytes());
            responseStream.flush();
            responseStream.close();
        } else { //404
            setResponseHeaders(404,0,request);
        }
    }

    private void handleGET(RequestParser request) throws IOException {
        File file = openFile(request);
        if (file.exists()){ //200
            InputStream fileInput = new FileInputStream(file);
            setResponseHeaders(200,file.length(),request);
            int len;
            while ((len = fileInput.read(responseBuffer)) != -1) {
                responseStream.write(responseBuffer, 0, len);
            }
            responseStream.flush();
            responseStream.close();
            fileInput.close();
        } else { //404
            setResponseHeaders(404,0,request);
        }
    }

    private void setResponseHeaders(int stateCode, long fileLength,
                                    RequestParser request) throws IOException {
        String contentType = "Content-Type: text/html; charset=utf-8\r\n";;
        switch (request.getFileType()){
            case "txt":
            case "java":
                contentType = "Content-Type: text/plain; charset=utf-8\r\n";
                break;
            case "htm":
            case "None":
            case "html":
                contentType = "Content-Type: text/html; charset=utf-8\r\n";
                break;
            case "jpg":
                contentType = "Content-Type: image/jpeg\r\n";
                break;
        }
        switch (stateCode){
            case 200:
                responseStream.write("HTTP/1.1 200 OK\r\n".getBytes());
                responseStream.write(contentType.getBytes());
                responseStream.write("Content-Length: ".getBytes());
                responseStream.write(String.valueOf(fileLength).getBytes());
                responseStream.write("\r\n\r\n".getBytes());
                break;
            case 404:
                responseStream.write("HTTP/1.1 404 file not found\r\n".getBytes());
                responseStream.write(contentType.getBytes());
                responseStream.write("Content-Length:22\r\n\r\n".getBytes());
                responseStream.write("<h1>404 Not Found</h1>".getBytes());
        }
    }

    private File openFile(RequestParser request){
        File file;
        switch (request.getFileType()) {
            case "txt":
            case "htm":
            case "java":
            case "html":
            case "jpg":
                file = new File(serverRoot+fileName);
                break;
            default:
                throw new IllegalStateException("Unexpected value: " + request.getFileType());
        }
        return file;
    }
}
