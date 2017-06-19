
import java.io.BufferedReader;
import java.io.IOException;

/**
 * Base de dados com proteção de acesso simultâneo
 * utilizando monitores.
 * @author G710
 */
public class Database {
    private static final int DATA_VECTOR_LENGTH = 100;
    
    private int[] data = new int[DATA_VECTOR_LENGTH];
    
    private AtomicCounter readerCount, writerCount;
    
    public Database(BufferedReader br) throws IOException {
        readerCount = new AtomicCounter(0);
        writerCount = new AtomicCounter(0);
        for (int i = 0; i < DATA_VECTOR_LENGTH; i++) {
            data[i] = Integer.parseInt(br.readLine());
        }
    }
    
    /**
     * Faz a leitura de um valor na base de dados.
     * @param position Posição no vetor
     * @param sleepTime Tempo fictício que a thread "demora" para ler
     * @return Valor lido do vetor
     * @throws InterruptedException 
     */
    public int read(int position, int sleepTime) throws InterruptedException {
        if (readerCount.getValue() == 0) // Caso seja o primeiro leitor, obtém o monitor deste objeto
            acquireAccess(0, 0, 0, false);
        
        int r;
        r = data[position];
        Thread.sleep(sleepTime);
        readerCount.decrement();
        
        if (readerCount.getValue() == 0) { // Se for o último leitor, indica que a leitura acabou
            finishedReading();
        }
        
        return r;
    }
    
    /**
     * Notifica todos as threads que esperam pelo monitor deste objeto.
     */
    private synchronized void finishedReading() {
        notifyAll();
    }
    
    /**
     * Inicia o procedimento de escrita na base de dados.
     * @param position Posição no vetor
     * @param value Valor a ser escrito
     * @param sleepTime Tempo fictício que a thread "demora" para escrever
     * @throws InterruptedException 
     */
    public void write(int position, int value, int sleepTime) throws InterruptedException {
        acquireAccess(position, value, sleepTime, true); // Adquire o monitor deste objeto
    }
    
    /**
     * Adquire o monitor deste objeto para iniciar o processo de escrita/leitura.
     * @param position Posição do vetor
     * @param value Valor a ser escrito, caso seja escrita
     * @param sleepTime Tempo fictício que a thread "demora"
     * @param write Determina se é um processo de escrita
     * @throws InterruptedException 
     */
    private synchronized void acquireAccess(int position, int value, int sleepTime, boolean write) throws InterruptedException {
        if (write) {
            synchWrite(position, value, sleepTime); // Caso seja um escritor tenta iniciar o processo de escrita
        }
        else {
            while (writerCount.getValue() > 0) { // Caso seja um leitor, espera até que o numero de escritores seja 0
                wait();
            }
            readerCount.increment();
        }
    }
    
    /**
     * Em posse do monitor deste objeto, escreve um valor na base de dados.
     * @param position Posição no vetor
     * @param value Valor a ser escrito
     * @param sleepTime Tempo fictício que a thread "demora" para escrever
     * @throws InterruptedException 
     */
    private synchronized void synchWrite(int position, int value, int sleepTime) throws InterruptedException {
        writerCount.increment();
        while (readerCount.getValue() > 0) { // Caso ainda haja leitores, espera
            wait();
        }
        data[position] = value;
        Thread.sleep(sleepTime);
        writerCount.decrement();
        notifyAll(); // Notifica as threads que esperam pelo monitor deste objeto
    }
    
    /**
     * Imprime o vetor da base de dados.
     */
    public void print() {
        for (int i = 0; i < DATA_VECTOR_LENGTH; i++) {
            System.out.print(data[i]+" ");
        }
    }
}
