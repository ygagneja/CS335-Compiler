// struct point {
// 	int x;
// 	int y;
// 	struct {int z; float p;} m;
// };

// struct aa {
// 	int s;
// 	char arr[10];
// 	struct point* a;
// };

// void chng(int* p){
// 	*p = 1;
// }

// struct point* func(struct point* p){
// 	p->x = 1;
// 	p->y = 2;
// 	return p;
// }
int glob = 1;
float glob2 = 2.3 + glob;

int func(int x){
	return x + glob;
}

int main(){
	float x = 2;
	int i = 10;
	bool p = true;
	float m = i*i + x*glob2;
	int ret = func(i);
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

bool glob3 = false;