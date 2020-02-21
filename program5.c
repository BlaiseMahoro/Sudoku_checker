#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sum; /* this data is shared by the thread(s) */
int checker[11];//used to keep track of what threads found out.
int repeat_checker[9];
struct parameters
{
   int row;    //start row
   int column; //start column
   int job;    //0 for subgrids, 1 for rows, 2 for colunns
};
void *runChecker(struct parameters *param); /* threads call this function */
int unique(int val);
void reset_array();

int arr_puzzle[9][9] = {{6, 2, 4, 5, 3, 9, 1, 8, 7}, 
                        {5, 1, 9, 7, 2, 8, 6, 3, 4}, 
                        {8, 3, 7, 6, 1, 4, 2, 9, 5}, 
                        {1, 4, 3, 8, 6, 5, 7, 2, 9},
                        {9, 5, 8, 2, 4, 7, 3, 6, 1}, 
                        {7, 6, 2, 3, 9, 1, 4, 5, 8},
                        {3, 7, 1, 9, 5, 6, 8, 4, 2}, 
                        {4, 9, 6, 1, 8, 2, 1, 7, 3},
                        {2, 8, 5, 4, 7, 3, 9, 1, 6}};
int h = 0;//used to know where we are.
int main(int argc, char *argv)
{

   pthread_t tid_arr[11];       /* the thread identifier */
   pthread_attr_t attr_arr[11]; /* set of thread attributes */

   struct parameters params;
   int i = 0, j = 0;
   for (; h < 11; h++)
   {
      
      
      if (h < 9) //Do grids
      {
         params.job = 0;
         params.row = i;
         params.column = j;
         printf("Grids \n");
        
         //For the 9 grids

         if (j == 6)
         {
            i += 3;
            j = 0;
         }
         else
            j += 3;
      }
      else if (h == 9)
      {
         //Rows
         printf("Rows\n");
         params.job = 1;
         params.row = 0;
         params.column = 0;
         
      }
      else
      {
         //Columns
         printf("Columns\n");
         params.job = 2;
         params.row = 0;
         params.column = 0;
      }
      pthread_attr_init(&attr_arr[h]);
      pthread_create(&tid_arr[h], &attr_arr[h], runChecker, &params);
      pthread_join(tid_arr[h], NULL);
   }
   int sum = 0;
   printf("\n\nCheck what all workers got...\n");
   for (int i = 0; i < 11; i++)
   {
      sum += checker[i];
      printf("%d, ", checker[i]);
   }
   printf("\nSum = %d\n\n", sum);
   if (sum == 11)
      printf("Sudoku is valid\n");
   else
      printf("Sudoku is not valid\n");
   return 0;
}
/* The thread will begin control in this function */
void *runChecker(struct parameters *params)
{

   //printf("%d %d %d\n", params->job, params->row, params->column);
   int r = params->row;//used to traverse
   int c = params->column; //used to traverse 
   if (params->job == 0)
   {
      reset_array();

      //Do grids
      for (int x = r; x < r + 3; x++)
      {
         
         for (int y = c; y < c + 3; y++)
         {
            printf("%d, ", arr_puzzle[x][y]);
            if ((arr_puzzle[x][y] < 1 || arr_puzzle[x][y] > 9)||! unique(arr_puzzle[x][y]))
            {
               printf("not valid\n");
               checker[h] = 0;
               return;
            }
         }
         printf("\n");
      }
   }
   else if (params->job == 1)
   {
      //Do rows

      for (int x = r; x < 9; x++)
      {
         reset_array();
         for (int y = c; y < 9; y++)
         {
            printf("%d, ", arr_puzzle[x][y]);

            if ((arr_puzzle[x][y] < 1 || arr_puzzle[x][y] > 9)|| !unique(arr_puzzle[x][y]))
            {

               printf("not valid\n");
               checker[h] = 0;
               return;
            }
         }
         printf("\n");
      }
   }
   else
   {
      //Do columns
      for (int x = r; x < 9; x++)
      {
         reset_array();
         for (int y = c; y < 9; y++)
         {
            printf("%d, ", arr_puzzle[y][x]);
            if ((arr_puzzle[y][x] < 1 || arr_puzzle[y][x] > 9) || !unique(arr_puzzle[y][x]))
            {
               printf("not valid!\n");
               checker[h] = 0;
               return;
            }
         }
         printf("\n");
      }
   }
   checker[h] = 1;//validity checker
   pthread_exit(0);
}

void reset_array(){
  for(int i=0; i<9;i++){
    repeat_checker[i]=1;
  }
}

//Ckecks if the number was seen.
int unique(int val){
   int found=0;
   int temp;
   
   if(repeat_checker[val-1]){
      repeat_checker[val-1]=0;
      return 1;
   }
      
   return found;

}
