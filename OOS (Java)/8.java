/*
 * 8) Implement a class for a “Book”. Book contains a title (a String), a list
 * of authors (array of authors), number of pages (an integer), price (floating
 * point number), publisher (a String) etc. Write suitable constructor and
 * accessor/modifier methods. Implement a class for “Library”. A library
 * contains a list of books (array of Book). Write add (to add a book) and
 * remove (to delete a book) methods for library. Write a main() function to
 * create a “Library” and add five “Book” to library. Print the total price of
 * all books.
 */

class Book {
    private String title, publisher;
    private String[] authors;
    private int num_pages;
    private double price;

    public Book(String title, String publisher, String[] authors, int num_pages, double price) {
        this.title = title;
        this.publisher = publisher;
        this.authors = authors;
        this.num_pages = num_pages;
        this.price = price;
    }

    public String getTitle() {
        return title;
    }

    public void setTitle(String title) {
        this.title = title;
    }

    public String getPublisher() {
        return publisher;
    }

    public void setPublisher(String publisher) {
        this.publisher = publisher;
    }

    public String[] getAuthors() {
        return authors;
    }

    public void setAuthors(String[] authors) {
        this.authors = authors;
    }

    public int getNum_pages() {
        return num_pages;
    }

    public void setNum_pages(int num_pages) {
        this.num_pages = num_pages;
    }



    public double getPrice() {
        return price;
    }

    public void setPrice(double price) {
        this.price = price;
    }
}