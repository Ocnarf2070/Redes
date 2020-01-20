package ClienteServidor;

import java.io.*;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

public class Cliente {
	public static void main(String[] args) {
		if (args.length != 2) {
			System.err.println("Error. Uso: java Cliente <ip> <port>");
			System.exit(0);
		}
		String ip = args[0];
		int port = Integer.parseInt(args[1]);
		try {
			// Crear socket
			System.out.println("Conectando a " + ip + ":" + port);
			Socket s = new Socket(ip, port);
			System.out.println("Conexion establecida");
			// Envio de peticion y recepcion de respuesta
			BufferedReader in = new BufferedReader(new InputStreamReader(s.getInputStream()));
			PrintWriter out = new PrintWriter(s.getOutputStream());
			byte[] mensaje_bytes = new byte[256];
			Scanner sc = new Scanner (System.in);
			String pal=sc.next();
			do {
				pal.toLowerCase();
				out.write(pal);
				in.readLine()();
			} while (pal=="terminar");
			out.write("BYE");
			in.read();
			// Deconexion
			out.close();
			in.close();
			sc.close();
			s.close();

		} catch (UnknownHostException e) {
			System.err.println("Error al crear socket. La IP del servidor no se pudo realizar");
		} catch (IOException e) {
			System.err.println("Error de conexión");
		}

	}
}
