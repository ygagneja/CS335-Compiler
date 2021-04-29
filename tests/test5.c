void printf(char* str, ...);
void scanf(char* str, ...);

struct Distance {
   int feet;
   float inch;
} d1, d2, result;

int main() {
   scanf("%d", &d1.feet);
   scanf("%f", &d1.inch);
 
   scanf("%d", &d2.feet);
   scanf("%f", &d2.inch);
   
   result.feet = d1.feet + d2.feet;
   result.inch = d1.inch + d2.inch;

   while (result.inch >= 12.0) {
      result.inch = result.inch - 12.0;
      ++result.feet;
   }
   printf("\nSum of distances = %d\'-%.1f\'", result.feet, result.inch);
   return 0;
}