struct point {
	int x;
	int y;
	struct {int z; float p;} m;
};

struct aa {
	int s;
	char arr[10];
	struct point* a;
};

struct point* func(float x, float y){
	struct point* ret;
	ret->x = x;
	ret->y = y;
	return ret;
}

int main(){
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
	int x = 1;
	float y = 2;
	char z = 'a';
	char* p = &z;
	bool a = true;

	int m = x*y + z/a - a;
	char* q = p + (int)a;
	int b = p + 1;
	b += y;

	switch (x){
		case 1 : {break;}
		case 2 : {continue;}
	}
	if ('c');
	return 0;
}
