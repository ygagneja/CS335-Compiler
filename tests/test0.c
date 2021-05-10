struct point {
	int x;
	int y;
	struct {int z; float p;} m;
};

struct aa {
	int s;
	struct point arr[10];
};

char* s = " ";

// void chng(int* p){
// 	*p = 1;
// }

// struct point* func(struct point* p){
// 	p->x = 1;
// 	p->y = 2;
// 	return p;
// }
// int glob = 1;
// float glob2 = 2.3 + glob;

// int func(int x){
// 	return x + glob;
// }
// void chng(struct point* p){
// 	p->m.p = sizeof(struct point);
// }
// void print(struct point str){
// 	str[2] = 'e';
// }
int main(){
	// struct aa x;
	// x.arr[2].y = 1;
	bool y;
	int i; char x;
	int arr[10];
	bool p;
	char* str = "hello";
	// print(str);
	x = str[2];
	// struct point {int x;} p;
	// {
	// 	struct test0{
	// 		int x1;
	// 	};
	// }
	// {
	// 	struct test0 x;
	// }
	// int arr[10][2];
	// int p = arr[1][0];
	// arr[3][1] = p;
	// int x = 1;
	// float y = 2;
	// char z = 'a';
	// char* p = &z;
	// bool a = true;

	// int m = x*y + z/a - a;
	// char* q = p + (int)a;
	// int b = p + 1;
	// b += y;

	// switch (x){
	// 	case 1 : {break;}
	// 	case 2 : {continue;}
	// }
	// if ('c');
	return 0;
}