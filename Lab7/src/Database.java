/* Classe que encapsula a variável, implementando os getters e setters com synchronized*/
public class Database {
	private int sharedVariable;
	
	Database() {
		this.sharedVariable = 0;
	}
	
	public synchronized int get() {
		return sharedVariable;
	}
	
	public synchronized void set(int value) {
		this.sharedVariable = value;
	}

}
