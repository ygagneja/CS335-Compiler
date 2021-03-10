struct Books {
   char  title[50];
   char  author[50];
   char  subject[100];
   int   book_id;
} book;  

enum week{Mon, Tue, Wed, Thur, Fri, Sat, Sun}; 

int main(){
    int a, b;
    int c;
    enum week day; 
    struct Books Book1;        
    struct Books Book2; 
    c = a + b;
    printf("%d",c);       
    
    strcpy( Book1.title, "CLRS");
    strcpy( Book1.author, "Bose"); 
    strcpy( Book1.subject, "Algo");
    Book1.book_id = 6495407;

    strcpy( Book2.title, "Quantum Physics");
    strcpy( Book2.author, "Senapati");
    strcpy( Book2.subject, "Physics");
    Book2.book_id = 6495700;
    
    printf( "Book 1 title : %s\n", Book1.title);
    printf( "Book 1 author : %s\n", Book1.author);
    printf( "Book 1 subject : %s\n", Book1.subject);
    printf( "Book 1 book_id : %d\n", Book1.book_id);

    printf( "Book 2 title : %s\n", Book2.title);
    printf( "Book 2 author : %s\n", Book2.author);
    printf( "Book 2 subject : %s\n", Book2.subject);
    printf( "Book 2 book_id : %d\n", Book2.book_id);

    day = Wed; 
    printf("%d",day); 

    return 0;
}