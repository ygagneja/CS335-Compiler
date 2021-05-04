struct point {
	int x;
	int y;
	struct {int z; double p;} m;
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
	int arr[10][2];
	int p = arr[1][0];
	arr[3][1] = p;
	return 0;
}
