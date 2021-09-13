/* Thread que l� a vari�vel e a imprime na tela indicando se � par ou �mpar e depois a altera para o dobro do seu valor atual*/
public class ReadWriteThread extends Thread {
	int id;
	RWPattern monitor;
	Database db;
	
	//Constructor
	ReadWriteThread(int identifier, RWPattern m, Database variable) {
		this.id = identifier;
		this.monitor = m;
		this.db = variable;
	}
	
	//M�todo executado pela thread
	public void run() {
		try {
			int interacao = 1;
			for(;;) {
				this.monitor.readLock(this.id);
				if((this.db.get() % 2) == 0) {
					System.out.printf("Thread RW %d - Interacao %d: %d � um n�mero par\n", this.id, interacao, this.db.get());
				} else {
					System.out.printf("Thread RW %d - Interacao %d: %d � um n�mero �mpar\n", this.id, interacao, this.db.get());
				}
				this.monitor.readUnlock(this.id);
				this.monitor.writeLock(this.id);
				this.db.set( this.db.get() * 2);
				System.out.printf("Thread RW %d - Interacao %d: A vari�vel agora � %d\n", this.id, interacao, this.db.get());
				this.monitor.writeUnlock(this.id);
				sleep(2000);
				interacao++;
			}
		}
		catch (InterruptedException e) {
			return;
		}
	}
}
