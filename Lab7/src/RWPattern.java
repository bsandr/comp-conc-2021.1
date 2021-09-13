/* Classe que implementa o padrão leitores/escritores */
public class RWPattern {
	
	private int readers, writers;
	
	//Constructor
	RWPattern(){
		this.readers = 0;
		this.writers = 0;
	}
	
	public synchronized void readLock(int id) {
		try {
			while(this.writers > 0) {
				System.out.printf("Leitor bloqueado %d\n", id);
				wait();
			}
			this.readers++;
			System.out.printf("Leitor lendo %d\n", id);
		} catch(InterruptedException e) {
			return;
		}
	}
	
	public synchronized void readUnlock(int id) {
		this.readers--;
		if(this.readers == 0) {
			this.notify();
		}
		System.out.printf("Leitor saindo %d\n", id);
	}
	
	public synchronized void writeLock(int id) {
		try {
			while((this.readers > 0) || (this.writers > 0)) {
				System.out.printf("Escritor bloqueado %d\n", id);
				wait();
			}
			
			this.writers++;
			System.out.printf("Escritor escrevendo %d\n", id);
		} catch (InterruptedException e) {
			return;
		}
	}
	
	public synchronized void writeUnlock(int id) {
		this.writers--;
		notifyAll();
		System.out.printf("Escritor saindo %d\n", id);
	}
 
}
