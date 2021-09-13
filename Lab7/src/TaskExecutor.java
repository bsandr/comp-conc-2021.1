/* Classe principal da aplicação */
public class TaskExecutor {
	static final int R = 10;
	static final int W = 10;
	static final int RW = 2;
	
	public static void main(String[] args) {
		int i;
		RWPattern monitor = new RWPattern();
		Database db = new Database();
		ReadThread[] r = new ReadThread[R];
		WriteThread[] w = new WriteThread[W];
		ReadWriteThread[] rw = new ReadWriteThread[RW];
		
		for(i = 0; i < R; i++) {
			r[i] = new ReadThread(i+1, monitor, db);
			r[i].start();
		}
		
		for(i = 0; i < W; i++) {
			w[i] = new WriteThread(i+1, monitor, db);
			w[i].start();
		}
		
		for(i = 0; i < RW; i++) {
			rw[i] = new ReadWriteThread(i+1, monitor, db);
			rw[i].start();
		}
	}
}
