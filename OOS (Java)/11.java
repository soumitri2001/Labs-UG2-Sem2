/* 
11) Create an abstract class “Publication” with data members ‘noOfPages’, ‘price’,
‘publisherName’ etc. with their accessor/modifier functions. Now create two sub-classes “Book”
and “Journal”. Create a class Library that contains a list of Publications. Write a main() function
and create three Books and two Journals, add them to library and print the details of all
publications.
*/

abstract class Publication {

    int noOfPages;
    long price;
    String publisherName;

    public int getNoOfPages() {
        return noOfPages;
    }

    public void setNoOfPages(int noOfPages) {
        this.noOfPages = noOfPages;
    }

    public long getPrice() {
        return price;
    }

    public void setPrice(long price) {
        this.price = price;
    }

    public String getPublisherName() {
        return publisherName;
    }

    public void setPublisherName(String publisherName) {
        this.publisherName = publisherName;
    }

    public Publication() {
    }

    public Publication(int noOfPages, long price, String publisherName) {
        this.noOfPages = noOfPages;
        this.price = price;
        this.publisherName = publisherName;
    }

    public void printDetails() {
        System.out.println("Name: " + publisherName);
        System.out.println("Price: " + price + " ,No.of pages: " + noOfPages);
        System.out.println();
    }
}

class Book extends Publication {

    public Book() {
    }

    public Book(int noOfPages, long price, String publisherName) {
        super(noOfPages, price, publisherName);
    }
}

class Journal extends Publication {
    public Journal() {
    }

    public Journal(int noOfPages, long price, String publisherName) {
        super(noOfPages, price, publisherName);
    }
}

class Library {

    public static void main(String[] args) {
        Journal j1 = new Journal(450, 100, "Springer");
        Journal j2 = new Journal(575, 90, "Elsevier");

        Book b1 = new Book(800, 900, "MIT Press");
        Book b2 = new Book(600, 1000, "McGraw-Hill");
        Book b3 = new Book(1000, 7000, "Oxford Press");

        j1.printDetails();
        j2.printDetails();

        b1.printDetails();
        b2.printDetails();
        b3.printDetails();
    }
}