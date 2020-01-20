package es.uma.informatica.rsd.chat.impl;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.InetSocketAddress;
import java.net.MulticastSocket;
import java.net.NetworkInterface;
import java.net.SocketAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import java.nio.charset.Charset;
import java.util.Arrays;
import java.util.Scanner;

import es.uma.informatica.rsd.chat.ifaces.Comunicacion;
import es.uma.informatica.rsd.chat.ifaces.Controlador;
import es.uma.informatica.rsd.chat.impl.DialogoPuerto.PuertoAlias;

/**
 * Implementación de prueba del componente de comunicación.
 * @author Francisco Chicano
 * @date 9/05/2014
 *
 */

public class ComunicacionImpl implements Comunicacion
{
	private Controlador c;
	private DatagramSocket ds; //No sirve, hereda de multicast
	private String alias;
	private int port;
	private MulticastSocket ms;
	private String msg;
	private InetAddress multi;
	
	
	
	public void crearSocket(PuertoAlias pa)
	{
		port = pa.puerto;
		alias = pa.alias;
		
		try {						
			ms = new MulticastSocket(port); //Creamos un socket multicast, que sirve tanto para comunicaciones
			//unicast como multicast.
		} catch (SocketException e) {			
			System.out.println("Ha ocurrido un error al crear el Socket.");
		}catch (IOException e){
			System.out.println("Error de entrada salida.");
		}
	}

	@Override
	public void setControlador(Controlador c)
	{
		this.c = c; 
	}

	@Override
	public void runReceptor()
	{				
		try {		
			
			while(true){			
				byte[] b = new byte[256];
				DatagramPacket dp = new DatagramPacket(b, b.length);
				ms.receive(dp);
				
				String msg_rec = new String(dp.getData(), Charset.forName("UTF-8"));
				Scanner sc = new Scanner(msg_rec);				
				sc.useDelimiter(",");				
				String first = sc.next();
								
				
				if(multi != null && first.equals(multi.getHostAddress())){	//Como la direccion que llega al DatagramPacket es una direccion local,
					//debo comprobar si es multicast usando el mensaje recibido
					String dir = first;
					String nick = sc.next();
					msg = sc.next();
					while(sc.hasNext()){
						msg += ", " +sc.next();
					}	
					SocketAddress sa = new InetSocketAddress(dir, port);
					if(NetworkInterface.getByInetAddress(dp.getAddress())!= null){
						continue; //Este metodo permite que al enviar un mensaje multicast, no aparezca enla pantalla de
						//quien envia. NetworkInterface identifica la interfaz local del equipounido a un grupo.
						//Si le llega el mensaje a alguien que no es quien ha enviado el mensaje, la interfaz es nula.
					}else{
						c.mostrarMensaje(sa, nick, msg);	
					}
				}else{ 
					String nick = first;
					msg = sc.next();
					while(sc.hasNext()){
						msg += ", " +sc.next();
					}	
					c.mostrarMensaje(dp.getSocketAddress(), nick, msg);
				}			
				sc.close();
			}
					
		} catch(IOException e){
			System.out.println("Error de entrada salida.");
		}
	
	}


	@Override
	public void envia(InetSocketAddress sa, String mensaje)
	{	//Dependiendo del tipo de chat, se envia el mensaje de una manera u otra.
		try{
			DatagramPacket dp;
			if(sa.getAddress().isMulticastAddress()){ 
				msg = new String (sa.getAddress().getHostAddress() + "," + alias  + "," + mensaje);							
			}else{
				msg = new String(alias + ", " + mensaje);					
			}		
			byte [] data = msg.getBytes(Charset.forName("UTF-8"));	
			dp = new DatagramPacket(data, data.length, sa.getAddress(),sa.getPort());
			ms.send(dp);
		} catch (IOException e) {			
			System.out.println("Error de entrada salida.");
		}
	
	}

	@Override
	public void joinGroup(InetAddress multi)
	{	
		try {	
			this.multi = multi;
			ms.joinGroup(multi);		
		} catch (IOException e) {			
			e.printStackTrace();
		}
		
	}

	@Override
	public void leaveGroup(InetAddress multi)
	{
		try {
			ms.leaveGroup(multi);
		} catch (IOException e) {			
			e.printStackTrace();
		}
	}

}