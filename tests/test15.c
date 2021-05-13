// Program to test lib functions

// int strlen(char* str);
// int isqrt(int num);
// float log10(float x);
// float floor(float x);
// int power(int x, int y);
// float sin(float x);
// float cos(float x);

// these functions are linked by linking the compiled code (using our compiler :) of those functions

int main(){
    char str[100];
    prints("Enter a string of size <= 100 :\n");
    scans(str, 100);
    prints("Length of the string is : "); printi(strlen(str)); prints("\n");
    prints("Enter an int to find isqrt :\n");
	printi(isqrt(scani())); prints("\n");
    prints("Enter element to find log10 :\n");
	printf(log10(scanf())); prints("\n");
    prints("Enter element to find floor :\n");
	printf(floor(scanf())); prints("\n");
    prints("Enter two ints x and y to find pow(x, y) :\n");
	printi(power(scani(), scani())); prints("\n");
    prints("Enter element to find sin :\n");
	printf(sin(scanf())); prints("\n");
    prints("Enter element to find cos :\n");
	printf(cos(scanf())); prints("\n");
}