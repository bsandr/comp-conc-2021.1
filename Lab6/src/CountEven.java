import java.lang.Thread;

public class CountEven extends Thread {
	private int id;
	Counter c;
	
	//Construtor
	public CountEven(int tid, Counter contador) {
		this.id = tid;
		this.c = contador;
	}
	public void run() {
		System.out.println("Thread " + this.id + " iniciou!");
		
		
		for(int i = id; i < CountEvenThread.num_elements ; i+= CountEvenThread.N) {
			if((CountEvenThread.vetor[i] % 2) == 0) {
				this.c.incrementEven();
			}
		}
		
		System.out.println("Thread " + this.id + " terminou!");
	}
}
