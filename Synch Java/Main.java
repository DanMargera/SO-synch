
import java.io.BufferedReader;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

/**
 * 
 * @author G710
 */
public class Main {
    public static void main(String[] args) {
        // Cria um leitor para o arquivo database.txt
        BufferedReader br;
        try {
            br = new BufferedReader(new FileReader("database.txt"));
        } catch (FileNotFoundException ex) {
            System.out.println(ex);
            return;
        }

        // Inicializa a base de dados a partir do arquivo
        Database db;
        try {
            db = new Database(br);
        } catch (IOException ex) {
            System.out.println(ex);
            return;
        }

        // Inicia a thread despachante
        Dispatcher dispatcher = new Dispatcher(db, br);
        dispatcher.start();


        // Espera a thread despachante finalizar
        try {
            dispatcher.join();
        } catch (InterruptedException ex) {
            System.out.println(ex);
        }

        System.out.println("\n");
        db.print();
    }
}
