package ClienteServidor;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.*;
import java.util.Scanner;

public class Servidor {

	static String reverse(String pal){
		String res = "";
		for(int i=0;i<pal.length();i++){
			res+=pal.charAt(pal.length()-i-1);
		}
		return res;
	}
	
	public static void main(String[] args) {
		if(args.length!=1){
			System.err.println("Error. Uso: java Servidor <port>");
			System.exit(0);
		}
		int port = Integer.parseInt(args[0]);
		try {
			while(true){
				ServerSocket ss=new ServerSocket(port,1);
				Socket cs=ss.accept();
				System.out.println("Conexion establecida con alguien");
				BufferedReader in = new BufferedReader(
						new InputStreamReader(cs.getInputStream()));
				PrintWriter out = new PrintWriter(cs.getOutputStream());
				//System.out.println("“Bienvenido al servicio de inversión de cadenas");
				do{
					String pal = in.readLine();
					System.out.println(pal);
					if(!in.equals("BYE")){
						String res;
						res=reverse(in.toString());
						out.write(res);
					}else out.write("OK");
						
				}while(in.equals("BYE"));
				ss.close();
			}
		} catch (IOException e) {
			System.err.println("No se ha abierto correctamente el servidor");
		}
	}

}
