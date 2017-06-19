

/**
 * 
 * @author G710
 */
public class Worker implements Runnable {
    
    Database db;
    Dispatcher dispatcher;
    private Thread thread;
    private int position, value, sleepTime;
    private boolean writer; // Variável de controle, determina se o trabalhador está escrevendo ou lendo
    
    public Worker(Database db, Dispatcher disp) {
        this.db = db;
        dispatcher = disp;
    }
    
    public Thread getThread() {
        return thread;
    }
    
    /**
     * Começa uma nova thread de leitura.
     * @param position Posição no vetor
     * @param sleepTime Tempo fictício que a thread "demora" para ler
     */
    public void startReading(int position, int sleepTime) {
        this.position = position;
        this.sleepTime = sleepTime;
        writer = false;
        thread = new Thread(this);
        thread.start();
    }
    
    /**
     * Começa uma nova thread de escrita.
     * @param position Posição no vetor
     * @param value Valor a ser escrito
     * @param sleepTime Tempo fictício que a thread "demora" para ler
     */
    public void startWriting(int position, int value, int sleepTime) {
        this.position = position;
        this.value = value;
        this.sleepTime = sleepTime;
        writer = true;
        thread = new Thread(this);
        thread.start();
    }
    
    @Override
    public void run() {
        try {
            
            if (writer)
                db.write(position, value, sleepTime);
            else
                value = db.read(position, sleepTime);
            
        } catch (InterruptedException ex) {
            System.out.println(ex);
        }
        
        // Notifica o despachante quando a Thread finaliza o trabalho
        dispatcher.jobsDone(this);
    }
}
