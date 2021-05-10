int glob = 1;

// void printi(int x);
// void printf(float x);
// void prints(char* str);
// int scani();
// float scanf();
// void scans(char* str, int len);
// void* malloc(int size);

int main(){
	char str[10];
	char* ptr;
	float y, res;
	int x;
	prints("enter a string");
	scans(str, 10); // length 10 means will read 8 chars, 1 \n, 1 \0
	prints("enter an int");
	x = scani();
	prints("enter a float");
	y = scanf();
	res = x + y*y;
	prints("result of x + y*y is");
	printf(res);
	prints(str);
	ptr = malloc(20);
	scans(ptr, 10);
	ptr[1] = ' ';
	prints(ptr);
	printi(ptr);
	printi(str);
	printi(&glob);
	return 0;
}