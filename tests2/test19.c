// File I/O program

void printf(char* str, ...);
void scanf(char* str, ...);
void fprintf(FILE* fptr, char* str, ...); // Check if this declaration is okay?
void fclose(FILE* fptr, ...);

int main()
{
   int num;
   FILE *fptr;

   // ab+ should create a file itself, hence not adding abc.txt separately
   fptr = fopen("abc.txt","ab+");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   printf("Enter num: ");
   scanf("%d",&num);

   fprintf(fptr,"%d",num);
   fclose(fptr);

   return 0;
}