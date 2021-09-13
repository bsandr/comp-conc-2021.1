/* Thread que modifica a vari�vel escrevendo o valor do seu id de thread*/
public class WriteThread extends Thread {
	int id;
	RWPattern monitor;
	Database db;
	
	//Constructor
	WriteThread(int identifier, RWPattern m, Database variable) {
		this.id = identifier;
		this.monitor = m;
		this.db = variable;
	}
	
	//M�todo executado pela thread
	public void run() {
		try {
			int interacao = 1;
			for(;;) {
				this.monitor.writeLock(this.id);
				this.db.set(this.id);
				System.out.printf("Thread W %d - Interacao %d: A vari�vel agora � %d\n", this.id, interacao, this.db.get());
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
