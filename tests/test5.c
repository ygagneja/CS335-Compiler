struct Distance {
   int feet;
   float inch;
} d1, d2, result;

int main() {
   // take first distance input
   scanf("%d", &d1.feet);
   scanf("%f", &d1.inch);
 
   // take second distance input
   scanf("%d", &d2.feet);
   scanf("%f", &d2.inch);
   
   // adding distances
   result.feet = d1.feet + d2.feet;
   result.inch = d1.inch + d2.inch;

   // convert inches to feet if greater than 12
   while (result.inch >= 12.0) {
      result.inch = result.inch - 12.0;
      ++result.feet;
   }
   printf("\nSum of distances = %d\'-%.1f\"", result.feet, result.inch);
   return 0;
}