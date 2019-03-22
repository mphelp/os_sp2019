// Name: Matthew Phelps
// Date: Feb 22 2019
#include "bitmap.h"

#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>
#include <string.h>
#include <complex.h>
#include <pthread.h>

/*
Convert a iteration number to an RGBA color.
Here, we just scale to gray with a maximum of max.
You can modify this function to make more interesting colors.
*/

int value_to_color( double complex z, int iter, int max )
{
  int gray = iter * 255 / max;
  return MAKE_RGBA(gray,gray,gray,0);
}

/*
Compute the number of iterations at point x, y
in the complex space, up to a maximum of max.
Return the color of the pixel at that point.

This example computes the Newton fractal:
z = z - alpha * p(z) / p'(z)

Assuming alpha = 1+i and p(z) = z^2 - 1 .

You can modify this function to match your selected fractal computation.
https://en.wikipedia.org/wiki/Newton_fractal
*/

static int compute_point( double x, double y, int max )
{
	// The initial value of z is given by x and y:
	double complex z = x + I*y;
	double complex alpha = 1 + I;

	int iter = 0;

	// Stop if magnitude too large or iterations exceed max
	while( cabs(z)<4 && iter < max ) {

		// Compute the new value of z from the previous:
		z = z - alpha*((cpow(z,2) - 1 ) / (2*z));
		iter++;
	}

	return value_to_color(z,iter,max);
}

void show_help()
{
	printf("Use: fractaltask [options]\n");
	printf("Where options are:\n");
	printf("-k <tasks>   Number of tasks for threads to complete. (default=1)\n");
	printf("-n <threads> Number of threads utilized. (default=1)\n");
	printf("-m <max>     The maximum number of iterations per point. (default=100)\n");
	printf("-x <coord>   X coordinate of image center point. (default=0)\n");
	printf("-y <coord>   Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>   Scale of the image in complex coordinates. (default=4)\n");
	printf("-W <pixels>  Width of the image in pixels. (default=640)\n");
	printf("-H <pixels>  Height of the image in pixels. (default=480)\n");
	printf("-o <file>    Set output file. (default=fractal.bmp)\n");
	printf("-h           Show this help text.\n");
}

// Modified task struct to be a linked list
// where each node has current task info, next task node ptr, and total task count
typedef struct Task {
	struct 	bitmap* bm;
	double 	xmin;
	double 	xmax;
	double 	ymin;
	double 	ymax;
	int 		max;
	int 	 	taskNum;
	int 		taskTotal;

	struct Task* next; // next task node
} Task;
typedef struct TaskQueue {
	Task* head;
} TaskQueue;

// Constructor
TaskQueue* TaskQueue_new(){
	TaskQueue* TQ = malloc(sizeof(TaskQueue));
	TQ->head = NULL;
	return TQ;
}
// Push to Task Queue
void TQpush(TaskQueue* TQ, Task* node){
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);
	if (TQ->head == NULL){
		TQ->head = node;
		pthread_mutex_unlock(&lock);
		return;
	}
	Task* prev = TQ->head;
	Task* curr = TQ->head->next;
	while (curr != NULL){
		prev = prev->next;
		curr = curr->next;
	}
	prev->next = node;
	pthread_mutex_unlock(&lock);
}
// Pop from Task Queue (NULL on empty queue)
Task* TQpop(TaskQueue* TQ){
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
	pthread_mutex_lock(&lock);
	if (TQ->head == NULL){
		pthread_mutex_unlock(&lock);
		return NULL;
	}
	Task* oldHead = TQ->head;
	TQ->head = TQ->head->next;
	pthread_mutex_unlock(&lock);
	return oldHead;
}	


// Modified compute_image so that each thread completes
// as many tasks (popped from queue) as are available
void* p_compute_image(void* arg){
	// Setup
	TaskQueue* TQ = (TaskQueue*) arg;
	Task* t;

	while ((t = TQpop(TQ)) != NULL){
		int i,j, startY, endY;

		int width = bitmap_width(t->bm);
		int height = bitmap_height(t->bm);

		// Modifications to height for concurrency
		startY = height/t->taskTotal*(t->taskNum);
		endY 	 = height/t->taskTotal*(t->taskNum+1);
		// For every pixel in the image...

		for(j=startY;j<endY;j++) {
			for(i=0;i<width;i++) {

				// Determine the point in x,y space for that pixel.
				double x = t->xmin + i*(t->xmax-t->xmin)/width;
				double y = t->ymin + j*(t->ymax-t->ymin)/height;
				
				// Compute the color at that point.
				int color = compute_point(x,y,t->max);

				// Set the pixel in the bitmap.
				bitmap_set(t->bm,i,j,color);
			}
		}
		// Make sure to free
		free(t);
	}	
	return NULL;
}
int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.

	int 	 threadTotal = 1;
	int 	 taskTotal = 1;
	const char *outfile = "fractaltask.bmp";

	double xcenter = 0;
	double ycenter = 0;
	double scale = 4;
	int    image_width = 640;
	int    image_height = 480;
	int    max = 100;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"k:n:x:y:s:W:H:m:o:h"))!=-1) {
		switch(c) {
			case 'k':
				taskTotal = atoi(optarg);
				break;
			case 'n':
				threadTotal = atoi(optarg);
				break;
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				scale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	// Display the configuration of the image.
	printf("fractaltask: n=%d k=%d x=%lf y=%lf scale=%lf max=%d outfile=%s\n",threadTotal,taskTotal,xcenter,ycenter,scale,max,outfile);

	// Create a bitmap of the appropriate size.
	struct bitmap *bm = bitmap_create(image_width,image_height);

	// Fill it with a dark blue, for debugging
	bitmap_reset(bm,MAKE_RGBA(0,0,255,0));

	// Setup task queue and threads
	pthread_t mythreads[threadTotal];
	TaskQueue* TQ = TaskQueue_new();

	for (int i = 0; i < taskTotal; i++){
		Task* t = malloc(sizeof(Task));
		t->bm = bm;
		t->taskTotal = taskTotal;
		t->taskNum = i;
		t->xmin = xcenter - scale;
		t->xmax = xcenter + scale;
		t->ymin = ycenter - scale;
		t->ymax = ycenter + scale;
		t->max  = max;
		t->next = NULL;

		TQpush(TQ, t); // add task to queue
	}
	// Create threads and give task queue to each thread
	for (int i = 0; i < threadTotal; i++){
		if ((pthread_create(&mythreads[i], NULL, 
						p_compute_image, (void*) TQ)) != 0){
			fprintf(stderr, "Thread couldn't be created: %s\n",strerror(errno));
			return 1;
		}
	}
	// Join threads at end
	for (int i = 0; i < threadTotal; i++){
		pthread_join(mythreads[i], NULL);
	}
	// Free up Task queue
	free(TQ);

	// Save the image in the stated file.
	if(!bitmap_save(bm,outfile)) {
		fprintf(stderr,"fractal: couldn't write to %s: %s\n",outfile,strerror(errno));
		return 1;
	}


	return 0;
}

