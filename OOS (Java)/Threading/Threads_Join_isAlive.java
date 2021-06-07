package Threading;

public class Threads_Join_isAlive {
    public static void main(String[] args) throws Exception {

        /* use of anonymous classes instead of creating separate classes */
        Runnable ob1 = new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 5; i++) {
                    System.out.println("Hi anonymous");
                    try {
                        Thread.sleep(1000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };

        Runnable ob2 = new Runnable() {
            @Override
            public void run() {
                for (int i = 0; i < 5; i++) {
                    System.out.println("Hello anonymous");
                    try {
                        Thread.sleep(1000);
                    } catch (Exception e) {
                        e.printStackTrace();
                    }
                }
            }
        };

        Thread t1 = new Thread(ob1);
        Thread t2 = new Thread(ob2);

        t1.start();
        try {
            Thread.sleep(10);
        } catch (Exception e) {
        }
        t2.start();

        t1.join(); // Main thread waits for t1 to join it back
        t2.join(); // Main thread waits for t2 to join it back

        // checking if threads t1 and t2 are still working or not
        System.out.println("Is t1 still alive ? " + t1.isAlive());
        System.out.println("Is t2 still alive ? " + t2.isAlive());

        // to be printed using Main thread (t1, t2 handles Hi and Hello)
        System.out.println("---------------------");

        /* use of Lambda expression instead of anonymous class */
        Runnable ob3 = () -> {
            for (int i = 0; i < 5; i++) {
                System.out.println("Hi Lambda");
                try {
                    Thread.sleep(1000);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };

        Runnable ob4 = () -> {
            for (int i = 0; i < 5; i++) {
                System.out.println("Hello Lambda");
                try {
                    Thread.sleep(1000);
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        };

        Thread t3 = new Thread(ob3);
        Thread t4 = new Thread(ob4);

        t3.start();
        try {
            Thread.sleep(10);
        } catch (Exception e) {
            e.printStackTrace();
        }
        t4.start();

        t3.join(); // Main thread waits for t3 to join it back
        t4.join(); // Main thread waits for t4 to join it back

        // checking if threads t1 and t2 are still working or not
        System.out.println("Is t3 still alive ? " + t3.isAlive());
        System.out.println("Is t4 still alive ? " + t4.isAlive());

        // to be printed using Main thread (t3, t4 handles Hi and Hello)
        System.out.println("<------ Bye ----->");
    }
}
