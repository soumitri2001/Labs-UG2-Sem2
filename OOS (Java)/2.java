/**
 * 2) Write a program to implement a class “student” with the following members.
 * Name of the student. Marks of the student obtained in three subjects.
 * Function to assign initial values. Function to compute total average.
 * Function to display the student’s name and the total marks. Write an
 * appropriate main() function to demonstrate the functioning of the above.
 */

class Student {
    private String name;
    private double marks[];

    public Student() {
        this.name = "";
        this.marks = new double[3];
    }

    public Student(String name, double marks[]) {
        this.name = name;
        this.marks = new double[marks.length];
        for (int i = 0; i < marks.length; i++)
            this.marks[i] = marks[i];
    }

    double average() {
        double avg = 0.0;
        for (int i = 0; i < this.marks.length; i++) {
            avg += this.marks[i];
        }
        return avg / this.marks.length;
    }

    void display() {
        System.out.println("Name: " + this.name);
        System.out.println("Marks obtained are: ");
        for (int i = 0; i < this.marks.length; i++)
            System.out.print(this.marks[i] + " ");
        System.out.println();
    }

    public static void main(String[] args) {
        double marks[] = { 97.5, 98.0, 94.5 };
        Student s = new Student("Soumitri Chattopadhyay", marks);
        s.display();
        System.out.println("Average marks = " + s.average());
    }
}