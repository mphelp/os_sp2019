
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
	printf("Use: fractalthread [options]\n");
	printf("Where options are:\n");
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

// All the tasks
typedef struct Task {
	struct 	bitmap* bm;
	double 	xmin;
	double 	xmax;
	double 	ymin;
	double 	ymax;
	int 		max;
	//int 		threadTotal;
	//int   	threadNum;
	int 	 	taskNum;
	int 		taskTotal;

	struct Task* next;
} Task;
/* TaskNode* TaskNode_new(Task* t){ */
/* 	TaskNode* node = malloc(sizeof(TaskNode)); */
/* 	node->val = t; */
/* 	node->next = NULL; */
/* 	return node; */
/* } */
typedef struct TaskQueue {
	/* TaskNode* head = TaskNode_new(NULL); */
	/* TaskNode* tail = TaskNode_new(NULL); */
	Task* head;
	void push(TaskNode* node){
		if (head == NULL){
			head = node;
			return;
		}
		Task* prev = head;
		Task* curr = head->next;
		while (curr != NULL){
			prev = prev->next;
			curr = curr->next;
		}
		prev->next = node;
	}
	bool isEmpty(){
		return (head == NULL);
	}
	Task* pop(){
		if (head == NULL){
			return head;
		}
		Task* oldHead = head;
		head = head->next;
		return oldHead;
	}	
} TaskQueue;
TaskQueue* TaskQueue_new(){
	TaskQueue* TQ = malloc(sizeof(TaskQueue));
	TQ->head = NULL;
	return TQ;
}
void printContents(Task* t){
	printf("xmin %f xmax %f ymin %f ymax %f max %d\n", T->xmin, T->xmax, T->ymin, T->ymax, T->max);
}
// TODO: Pass tasks queue here not threads
void* p_compute_image(void* arg){
	/* Task* t = (Task*) arg; */
	// Task Queue setup...
	TaskQueue* TQ = (TaskQueue*) arg;
	Task* t;
	while (t = TQ.pop()){
		int i,j, startY, endY;

		int width = bitmap_width(t->bm);
		int height = bitmap_height(t->bm);

		// Modifications to height for concurrency
		startY = height/t->threadTotal*(t->threadNum);
		endY 	 = height/t->threadTotal*(t->threadNum+1);
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
}
int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.

	int 	 threadTotal = 1;
	const char *outfile = "fractaltask.bmp";

	double xcenter = 0;
	double ycenter = 0;
	double scale = 4;
	int    image_width = 640;
	int    image_height = 480;
	int    max = 100;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"n:x:y:s:W:H:m:o:h"))!=-1) {
		switch(c) {
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
	printf("fractal: n=%d x=%lf y=%lf scale=%lf max=%d outfile=%s\n",threadTotal,xcenter,ycenter,scale,max,outfile);

	// Create a bitmap of the appropriate size.
	struct bitmap *bm = bitmap_create(image_width,image_height);

	// Fill it with a dark blue, for debugging
	bitmap_reset(bm,MAKE_RGBA(0,0,255,0));

	// TODO TODO TODO!!!
	// Setup tasks and threads
	pthread_t mythreads[threadTotal];
	Task* mytasks[threadTotal];

	for (int i = 0; i < threadTotal; i++){
		mytasks[i] = malloc(sizeof(Task));
		mytasks[i]->bm = bm;
		mytasks[i]->threadTotal = threadTotal;
		mytasks[i]->threadNum = i;
		mytasks[i]->xmin = xcenter - scale;
		mytasks[i]->xmax = xcenter + scale;
		mytasks[i]->ymin = ycenter - scale;
		mytasks[i]->ymax = ycenter + scale;
		mytasks[i]->max  = max;
	}
	// Create threads -- loop is based on tasks
	for (int i = 0; i < threadTotal; i++){
		if ((pthread_create(&mythreads[i], NULL, 
						p_compute_image, (void*) mytasks[i])) != 0){
			fprintf(stderr, "Thread couldn't be created: %s\n",strerror(errno));
			return 1;
		}
	}
	// Join threads
	for (int i = 0; i < threadTotal; i++){
		pthread_join(mythreads[i], NULL);
	}
	// Free up tasks
	for (int i = 0; i < threadTotal; i++){
		free(mytasks[i]);
	}

	// Save the image in the stated file.
	if(!bitmap_save(bm,outfile)) {
		fprintf(stderr,"fractal: couldn't write to %s: %s\n",outfile,strerror(errno));
		return 1;
	}


	return 0;
}

