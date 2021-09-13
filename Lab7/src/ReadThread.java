/* Thread que l� a vari�vel e a imprime na tela indicando se � um n�mero primo ou n�o*/
public class ReadThread extends Thread {
	int id;
	RWPattern monitor;
	Database db;
	
	//Constructor
	ReadThread(int identifier, RWPattern m, Database variable) {
		this.id = identifier;
		this.monitor = m;
		this.db = variable;
	}
	
	//M�todo executado pela thread
	public void run() {
		try {
			int interacao = 1;
			for(;;) {
				boolean isPrime = true;
				this.monitor.readLock(this.id);
				if (this.db.get() > 1) {
					for (int i = 2; i <= this.db.get()/2; i++) {
						if((this.db.get() % i) == 0) {
							isPrime = false;
							break;
						}
					}
					if(isPrime) {
						System.out.printf("Thread R %d - Interacao %d: %d � um n�mero primo\n", this.id, interacao, this.db.get());
					} else {
						System.out.printf("Thread R %d - Interacao %d: %d n�o � um n�mero primo\n", this.id, interacao, this.db.get());
					}
				} else {
					System.out.printf("Thread R %d - Interacao %d: %d n�o � um n�mero primo\n", this.id, interacao, this.db.get());
				}
				
				this.monitor.readUnlock(this.id);
				sleep(2000);
				interacao++;
			}
		}
		catch (InterruptedException e) {
			return;
		}
	}
}
