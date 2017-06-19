
/**
 * Implementação de um mutex utilizando monitores.
 * @author G710
 */
public class AtomicCounter {
    private static enum AccessType { READ, INCREMENT, DECREMENT; };
    private int counter;
    
    public AtomicCounter(int initialValue) {
        counter = initialValue;
    }
    
    public int getValue() {
        return access(AccessType.READ);
    }
    
    public void increment() {
        access(AccessType.INCREMENT);
    }
    
    public void decrement() {
        access(AccessType.DECREMENT);
    }
    
    private synchronized int access(AccessType type) {
        switch (type) {
            case INCREMENT:
                counter++;
                break;
            case DECREMENT:
                counter--;
                break;
            case READ:
                return counter;
        }
        return -1;
    }
}
