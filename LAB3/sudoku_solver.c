/*
 * Sudoku Solver Program for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <Mahum, Cassidy, Shivani>
 * All rights reserved.
 *
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define bufSize 1024

struct thread_args {
  int x;
  int y;
};
int only_answer;
int puzzle[9][9];
int possibilites[9][9][9];
int rpossibilites[9][9][9];
int gpossibilites[9][9][9];
int cpossibilites[9][9][9];
int set[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9};

int is_complete(){
	int notdone=0;
	int done=1;
	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {

			if (puzzle[x][y]==0){
				return notdone;
			}
		}
		break;
		return done;
	}



}
void *sub_grid_solver(void *input) {
  int count = 0;
  int size = 0;
  if (((struct thread_args *)input)->x < 3) {
    ((struct thread_args *)input)->x = 0;
  } else if (((struct thread_args *)input)->x < 6) {
    ((struct thread_args *)input)->x = 3;
  } else {
    ((struct thread_args *)input)->x = 6;
  }
  if (((struct thread_args *)input)->y < 3) {
    ((struct thread_args *)input)->y = 0;
  } else if (((struct thread_args *)input)->y < 6) {
    ((struct thread_args *)input)->y = 3;
  } else {
    ((struct thread_args *)input)->y = 6;
  }

  for (int i = ((struct thread_args *)input)->x;
       i < ((struct thread_args *)input)->x + 3; i++) {
    for (int j = ((struct thread_args *)input)->y;
         j < ((struct thread_args *)input)->y + 3; j++) {
      if (puzzle[i][j] != 0) {
        size++;
      }
    }
  }
  int values[size];
  for (int i = ((struct thread_args *)input)->x;
       i < ((struct thread_args *)input)->x + 3; i++) {
    for (int j = ((struct thread_args *)input)->y;
         j < ((struct thread_args *)input)->y + 3; j++) {
      if (puzzle[i][j] != 0) {
        values[count] = puzzle[i][j];
        count++;
      }
    }
  }

  for (int i = 0; i < 9; i++) {
    int j;
    for (j = 0; j < size; j++) {
      if (set[i] == values[j]) {
        break;
      }
    }
    if (j == size) {
      // printf("%d ",set[i] );
      gpossibilites[((struct thread_args *)input)->x]
                   [((struct thread_args *)input)->y][count] = set[i];
      count++;
    }
  }
  // printf("%s\n", "grid: ");
  // for (int j = 0; j < 9; j++) {
  //   printf("%d ", gpossibilites[((struct thread_args *)input)->x]
  //                              [((struct thread_args *)input)->y][j]);
  // }
  // printf("\n");
}
void *column_solver(void *input) {
  int count = 0;
  int size = 0;
  int j;

  // find out how many numbers are already in the row
  for (int i = 0; i < 9; i++) {
    if (puzzle[i][((struct thread_args *)input)->y] != 0) {
      size++;
    }
  }
  int values[size];
  for (int i = 0; i < 9; i++) {
    if (puzzle[i][((struct thread_args *)input)->y] != 0) {
      values[count] = puzzle[i][((struct thread_args *)input)->y];
      count++;
    }
  }
  for (int j = 0; j < size; j++) {
    // printf("%d ", values[j] );
  }

  for (int i = 0; i < 9; i++) {
    int j;
    for (j = 0; j < size; j++) {
      if (set[i] == values[j]) {
        break;
      }
    }
    if (j == size) {
      // printf("%d ",set[i] );
      cpossibilites[((struct thread_args *)input)->x]
                   [((struct thread_args *)input)->y][count] = set[i];
      count++;
    }
  }
  // printf("column possibilites for %d: \n",
  //        (((struct thread_args *)input)->y) + 1);

  // for (int j = 0; j < 9; j++) {
  //   printf("%d ", cpossibilites[((struct thread_args *)input)->x]
  //                              [((struct thread_args *)input)->x][j]);
  // }
  // printf("\n");
}
void *row_solver(void *input) {
  //	struct arg_struct *args = arguments;
  int count = 0;
  int size = 0;
  int j;

  // find out how many numbers are already in the row
  for (int i = 0; i < 9; i++) {
    if (puzzle[((struct thread_args *)input)->x][i] != 0) {
      size++;
    }
  }
  int values[size];
  for (int i = 0; i < 9; i++) {
    if (puzzle[((struct thread_args *)input)->x][i] != 0) {
      values[count] = puzzle[((struct thread_args *)input)->x][i];
      count++;
    }
  }
  for (int j = 0; j < size; j++) {
    // printf("%d ", values[j] );
  }

  for (int i = 0; i < 9; i++) {
    int j;
    for (j = 0; j < size; j++) {
      if (set[i] == values[j]) {
        break;
      }
    }
    if (j == size) {
      // printf("%d ",set[i] );
      rpossibilites[((struct thread_args *)input)->x]
                   [((struct thread_args *)input)->y][count] = set[i];
      count++;
    }
  }
  // printf("row possibilites for %d: \n", (((struct thread_args *)input)->x) + 1);
  // for (int j = 0; j < 9; j++) {
  //   printf("%d ", rpossibilites[((struct thread_args *)input)->x]
  //                              [((struct thread_args *)input)->y][j]);
  // }
  // printf("\n");
}
void printPuzzle() {
  // print out 2D array
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      printf("%d ", puzzle[i][j]);
    }
    printf("\n");
  }
  printf("\n\n");
}
void updatePossibilities() {
  int size = 0;
  int i;
  pthread_t tid1, tid2, tid3;
  struct thread_args *args =
      (struct thread_args *)malloc(sizeof(struct thread_args));

  for (i = 0; i < 9; i++) {
    args->x = i;
    for (int j = 0; j < 9; j++) {
      args->y = j;
      pthread_create(&tid1, NULL, row_solver, (void *)args);
      pthread_join(tid1, NULL);
    }
  }

  for (i = 0; i < 9; i++) {
    args->x = i;
    for (int j = 0; j < 9; j++) {
      args->y = j;
      pthread_create(&tid2, NULL, column_solver, (void *)args);
      pthread_join(tid2, NULL);
    }
  }

  for (i = 0; i < 9; i++) {
    args->x = i;
    for (int j = 0; j < 9; j++) {
      args->y = j;
      pthread_create(&tid3, NULL, sub_grid_solver, (void *)args);
      pthread_join(tid3, NULL);
    }
  }
	free(args);
}
int *cell_solver(int x, int y) {

  updatePossibilities();
	int *finresult = (int *)malloc(sizeof(int));
  int *result = (int *)malloc(sizeof(int));
  int result_size = 0;

  for (int i = 0; i < 9; i++) {
    int element = cpossibilites[x][y][i];

    for (int j = 0; j < 9; j++) {
      if ((element == rpossibilites[x][y][j]) && element != 0) {
        result[result_size] = element;
        result_size++;
      }
    }
  }
  // printf("common elements are:\n");
	//
  // for (int i = 0; i < result_size; i++) {
  //   printf("%d ", result[i]);
  // }
  if (x < 3) {
    x = 0;
  } else if (x < 6) {
    x = 3;
  } else {
    x = 6;
  }
  if (y < 3) {
    y = 0;
  } else if (y < 6) {
    y = 3;
  } else {
    y = 6;
  }

  int newresult_size = 0;

  for (int i = 0; i < 9; i++) {
    int element = gpossibilites[x][y][i];

    for (int j = 0; j < result_size; j++) {
      if ((element == result[j]) && element != 0) {
        finresult[newresult_size] = element;
        newresult_size++;
      }
    }
  }

  // printf("common elements are:\n");
	//
  // for (int i = 0; i < newresult_size; i++) {
  //   printf("%d ", finresult[i]);
  // }

  only_answer = newresult_size;
	return finresult;
	free(finresult);
	free(result);
}
void sudoku_solver(){

	for (int x = 0; x < 9; x++) {
		for (int y = 0; y < 9; y++) {

			if (puzzle[x][y]==0){
			int	*p =cell_solver(x,y);
				if(only_answer==1){
					puzzle[x][y]=*(p+0);
				}
			}
		}
	}




}
int main(void) {

  FILE *fp;
  fp = fopen("puzzle.txt", "r");
  char *token;
  char line[bufSize];

  for (int i = 0; i < 9; i++) {
    // read each line and print and save as line
    fgets(line, sizeof(line), fp);
    line[strlen(line) - 1] = '\0';
    // printf("%s\n", line);

    // use strtok to populate 2Darray
    token = strtok(line, " ");
    int j = 0;
    while (token != NULL) {
      puzzle[i][j] = atoi(token);
      j++;
      token = strtok(NULL, " ");
    }
  }

  printPuzzle();

	// for (int x = 0; x < 9; x++) {
	// 	for (int y = 0; y < 9; y++) {
	// 		if (puzzle[x][y]==0){
	// 		int	*p =cell_solver(x,y);
	// 			if(only_answer==1){
	// 				puzzle[x][y]=*(p+0);
	// 			}
	// 		}
	// 	}
	// }

sudoku_solver();
sudoku_solver();
sudoku_solver();
sudoku_solver();


// 	while(1){
// 	//sudoku_solver();
// 	printf("%s\n","hi" );
// 	int done=is_complete();
// 	if(done==1){
// 		break;
// 	}
// }
	// for (int x = 0; x < 9; x++) {
	// 	for (int y = 0; y < 9; y++) {
	//
	// 		if (puzzle[x][y]==0){
	// 			sudoku_solver();
	// 			break;
	// 		}
	// 		else
	// 		{
	// 			printf("%s\n", "puzzle solved!");
	// 			break;
	// 	}
	// 	}
	// }


		printPuzzle();

  //cell_solver(2, 0);

  pthread_exit(NULL);
  return 0;
}

