import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.ParseException;
import java.util.HashMap;
import java.util.Map;

/*
This class is will parse the input stream of the request,
split and store its attributes in the class itself.

This class also contains some methods to provide the extracted information.
 */
public class RequestParser {
    private final String method;
    private final String source;
    private final String fileName;
    private final String protocol;
    private final String type;
    private final Map<String,String> headerMap = new HashMap<String, String>();
    private final Map<String,String> requestMap = new HashMap<String, String>();
    private String tmpLine;
    private static final int BUFFER_LEN = 8192;
    RequestParser(InputStream request) throws IOException, ParseException {
        BufferedReader reader = new BufferedReader(new InputStreamReader(request));
        tmpLine = reader.readLine();
        String[] params = tmpLine.split(" ");
        if (params.length != 3){ //Make sure the first line contains 3 items.
            throw new ParseException("Invalid Request!",0);
        }
        method = params[0];
        source = params[1];
        protocol = params[2];

        int index = source.lastIndexOf('.');
        if (index == -1){
            type = "None";
        } else {
            type = source.substring(source.lastIndexOf('.')+1);
        }
        index = source.lastIndexOf('/');
        if (source.length() > 1){
            fileName = source.substring(index + 1);
        } else {
            fileName = "";
        }
        parseHeaders(reader);

        if (method.equals("POST")){
            parseBody(reader);
        }
    }



    public String getMethod(){
        return method;
    }

    public String getSource(){ return source; }

    public String getFileName(){ return fileName; }

    public String getFileType(){
        return type;
    }

    public String getFilePath(){
        int index = source.indexOf("/");
        if (index == -1){
            return source;
        } else {
            return source.substring(index);
        }
    }

    public String getProtocol(){
        return protocol;
    }

    public Map<String,String> getHeaderMap(){
        return headerMap;
    }

    public Map<String,String> getRequestMap(){
        return requestMap;
    }

    private void parseHeaders(BufferedReader reader) throws IOException {
        String key;
        String value;
        while ((tmpLine = reader.readLine()) != null){
            int index = tmpLine.indexOf(": ");
            if (index > 0){
                key = tmpLine.substring(0,index);
                value = tmpLine.substring(index+2);
                headerMap.put(key,value);
            }else {
                break;
            }
        }
    }

    private void parseBody(BufferedReader reader) throws IOException {
        String key;
        String value;
        char[] buffer = new char[BUFFER_LEN];
        int len = Integer.parseInt(headerMap.get("Content-Length"));
        reader.read(buffer,0,len);
        String[] rawData = new String(buffer).substring(0,len).split("&");
        String[] dataMap;
        for (String item : rawData){
            dataMap = item.split("=");
            key = dataMap[0];
            value = dataMap.length > 1 ? dataMap[1] : "";
            requestMap.put(key, value);
        }
    }
}
