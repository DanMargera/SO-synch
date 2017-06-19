
import java.io.BufferedReader;
import java.io.IOException;
import java.util.ArrayList;

/**
 * Classe despachante.
 * Realiza o despacho de trabalhadores para realizar leitura e escrita na base de dados.
 * @author G710
 */
public class Dispatcher extends Thread {
    private static final int MAX_WORKERS = 10, FIRST = 0;
    
    private Worker[] workers;
    private ArrayList<Worker> idleWorkers; // Lista contendo somente os trabalhadores ociosos no momento
    private BufferedReader src; // Fonte das requisições de escrita/leitura
    
    public Dispatcher(Database db, BufferedReader br) {
        src = br;
        workers = new Worker[MAX_WORKERS];
        idleWorkers = new ArrayList<>();
        idleWorkers.ensureCapacity(MAX_WORKERS);
        for (int i = 0; i < MAX_WORKERS; i++) {
            workers[i] = new Worker(db, this);
            idleWorkers.add(workers[i]);
        }
    }
    
    /**
     * Lógica de execução do despachante.
     */
    @Override
    public void run() {
        try {
            String line;
            String split[];
            int pos, val, sleep;
            // Faz a leitura da fonte e processa as requisições para realizar o despacho
            while ((line = src.readLine()) != null) {
                split = line.split(" ");
                pos = Integer.parseInt(split[1]);
                sleep = Integer.parseInt(split[2]);
                val = Integer.parseInt(split[3]);
                System.out.println(split[0].charAt(0)+" "+pos+" "+sleep+" "+val);
                switch (split[0].charAt(0)) {
                    case 'E':
                        dispatchWriter(pos, val, sleep);
                        break;
                    case 'L':
                        dispatchReader(pos, sleep);
                        break;
                }
            }
            System.out.println("End of requests.\nFinishing Execution...");
             // Ao fim das requisições, espera pelo fim das Threads trabalhadoras
            for (int i = 0; i < MAX_WORKERS; i++) {
                workers[i].getThread().join();
            }
        } catch (IOException | InterruptedException ex) {
            System.out.println(ex);
        }
        
    }
    
    /**
     * Despacha um trabalhador para leitura da base de dados.
     * @param position Posição no vetor
     * @param sleepTime Tempo fictício que a thread "demora" para ler
     * @throws InterruptedException 
     */
    private synchronized void dispatchReader(int position, int sleepTime) throws InterruptedException {
        System.out.println("Seeking/Waiting available workers...");
        while (idleWorkers.isEmpty()) { // Espera enquanto não há trabalhadores ociosos
            wait();
        }
        Worker w = idleWorkers.get(FIRST);
        w.startReading(position, sleepTime);
        idleWorkers.remove(w);
    }
    
    /**
     * Despacha um trabalhador para escrita na base de dados.
     * @param position Posição no vetor
     * @param value Valor a ser escrito
     * @param sleepTime Tempo fictício que a thread "demora" para ler
     * @throws InterruptedException 
     */
    private synchronized void dispatchWriter(int position, int value, int sleepTime) throws InterruptedException {
        System.out.println("Seeking/Waiting available workers...");
        while (idleWorkers.isEmpty()) { // Espera enquanto não há trabalhadores ociosos
            wait();
        }
        Worker w = idleWorkers.get(FIRST);
        w.startWriting(position, value, sleepTime);
        idleWorkers.remove(w);
    }
    
    /**
     * Método chamado por uma thread trabalhadora que finalizou sua execução.
     * @param w Trabalhador que finalizou sua thread
     */
    public synchronized void jobsDone(Worker w) {
        idleWorkers.add(w);
        notify(); // Notifica a thread despachante caso ela esteja esperando trabalhadores ociosos.
    }
}
