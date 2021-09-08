public class Counter {
	private int even = 0;

	public synchronized int getEven() {
		return even;
	}

	public synchronized void incrementEven() {
		this.even++;
	}
}
