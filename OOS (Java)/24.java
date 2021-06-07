class SeriesSum {

    static int number = 0;
    static int denom = 1;
    static final int LIMIT = 10;

    static double sum = 0.0;

    private int factorial(int x) {
        return x <= 1 ? 1 : x * factorial(x - 1);
    }

    private void seriesSum() {
        synchronized (this) {
            sum += (1.0 / denom);
            // number++;
            // Notify to second thread
            notify();
        }
        // }
    }

    private void denominator() {
        synchronized (this) {
            denom = factorial(number);
            number++;
            // Notify to 2nd thread
            notify();
        }
    }

    public static void main(String[] args) throws Exception {

        SeriesSum obj = new SeriesSum();

        Thread odd = new Thread(new Runnable() {
            public void run() {
                obj.denominator();
            };
        });

        Thread even = new Thread(new Runnable() {
            public void run() {
                obj.seriesSum();
            };
        });

        odd.start();
        even.start();

        odd.join();
        even.join();

        System.out.println("Total sum = " + sum);
    }
}