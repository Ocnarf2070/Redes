package pr;

import java.util.*;

public class rev {
	static String reverse(String pal){
		String res = "";
		for(int i=0;i<pal.length();i++){
			res+=pal.charAt(pal.length()-i-1);
		}
		return res;
	}
	public static void main(String[] args) {
		System.out.print("Introduce una palabra:");
		try(Scanner scan=new Scanner(System.in)){
		String pal=scan.next();
		System.out.println(reverse(pal));
		}catch(Exception e){
			System.err.println("No es un String");
		}
	}
}
