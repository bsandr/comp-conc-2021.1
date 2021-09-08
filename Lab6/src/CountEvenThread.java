import java.lang.Thread;
import java.util.Random;

//Classe do metodo main
public class CountEvenThread {
	static int N;
	static int num_elements;
	static int seq_count = 0;
	public static int[] vetor; 
	
	public static void main(String[] args) {
		
		if(args.length < 2) {
			System.err.printf("Digite: java CountEvenThread <numero de threads> <tamanho do vetor>");
			System.exit(1);
		}
		
		N = Integer.parseInt(args[0]);
		num_elements = Integer.parseInt(args[1]);
		
		//Aloca os valores do array
		vetor = new int[num_elements];
		Random rand = new Random();
		for(int i = 0; i < vetor.length; i++) {
			vetor[i] = rand.nextInt();
		}
		
		for(int i = 0; i < vetor.length; i++) {
			if((vetor[i] % 2) == 0) {
				seq_count++;
			}
		}
		
		//Reserva espaço para as threads
		Thread[] threads = new Thread[N];
		
		//Cria instancia do recurso compartilhado
		Counter count = new Counter();
		
		//Cria threads
		for(int i = 0; i < threads.length; i++) {
			threads[i] = new CountEven(i, count);
		}
		
		//Inicia as threads
		for(int i = 0; i < threads.length; i++) {
			threads[i].start();
		}
		
		//Espera o término das threads
		for(int i = 0; i < threads.length; i++) {
			try {
				threads[i].join();
			} catch(InterruptedException e) {
				return;
			}
		}
		
		System.out.printf("---- Resultado sequencial: %d -------\n", seq_count);
		System.out.printf("---- Resultado concorrente: %d ------\n", count.getEven());
		System.out.println("Terminou");
	}
}
