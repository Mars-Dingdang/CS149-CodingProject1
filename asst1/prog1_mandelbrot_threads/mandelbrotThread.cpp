#include <stdio.h>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>
#include <thread>

#include "CycleTimer.h"

typedef struct {
    float x0, x1;
    float y0, y1;
    unsigned int width;
    unsigned int height;
    int maxIterations;
    int* output;
    int threadId;
    int numThreads;
} WorkerArgs;


extern void mandelbrotSerial(
    float x0, float y0, float x1, float y1,
    int width, int height,
    int startRow, int numRows,
    int maxIterations,
    int output[]);


//
// workerThreadStart --
//
// Thread entrypoint.

void workerThreadStart(WorkerArgs * const args) {

    // TODO FOR CS149 STUDENTS: Implement the body of the worker
    // thread here. Each thread should make a call to mandelbrotSerial()
    // to compute a part of the output image.  For example, in a
    // program that uses two threads, thread 0 could compute the top
    // half of the image and thread 1 could compute the bottom half.

// Then, measure the amount of time
//   each thread requires to complete its work by inserting timing code at
//   the beginning and end of `workerThreadStart()`. How do your measurements
//   explain the speedup graph you previously created?

    double startTime = CycleTimer::currentSeconds();

    // New workload distribution strategy using striped allocation:
    // Instead of allocating contiguous blocks of rows to each thread,
    // we'll use a striped approach where each thread processes every
    // nth row (where n = numThreads). This ensures that each thread
    // gets a mix of simple and complex rows, leading to better load balancing.
    
    int totalRows = args->height;
    int numThreads = args->numThreads;
    int threadId = args->threadId;
    
    // In stripe allocation, each thread processes rows at positions:
    // threadId, threadId + numThreads, threadId + 2*numThreads, ...
    
    // Count how many rows this thread will process
    // int numRows = totalRows - 1 - threadId; // Total rows minus the rows before this thread's first row
    // numRows = (numRows / numThreads) + 1; // Divide by num
    // for (int row = threadId; row < totalRows; row += numThreads) {
    //     numRows++;
    // }

    for(int row = threadId; row < totalRows; row += numThreads) {
        mandelbrotSerial(args->x0, args->y0, args->x1, args->y1, 
                         args->width, args->height, row, 1, 
                         args->maxIterations, args->output);
    }

    int numRows = (totalRows + numThreads - 1 - threadId) / numThreads; // This formula calculates the number of rows for the thread based on its ID and total rows
    
    // // Allocate memory for the rows this thread will process
    // int* rowIndices = new int[numRows];
    // int idx = 0;
    // for (int row = threadId; row < totalRows; row += numThreads) {
    //     rowIndices[idx++] = row;
    // }
    
    // // Process each row assigned to this thread
    // for (int i = 0; i < numRows; i++) {
    //     int row = rowIndices[i];
    //     mandelbrotSerial(args->x0, args->y0, args->x1, args->y1, 
    //                      args->width, args->height, row, 1, 
    //                      args->maxIterations, args->output);
    // }
    
    // // Clean up
    // delete[] rowIndices;

    double endTime = CycleTimer::currentSeconds();
    printf("Thread %d completed in %.3f ms (processed %d rows in stripes)\n", args->threadId, (endTime - startTime) * 1000, numRows);
}

//
// MandelbrotThread --
//
// Multi-threaded implementation of mandelbrot set image generation.
// Threads of execution are created by spawning std::threads.
void mandelbrotThread(
    int numThreads,
    float x0, float y0, float x1, float y1,
    int width, int height,
    int maxIterations, int output[])
{
    static constexpr int MAX_THREADS = 32;

    if (numThreads > MAX_THREADS)
    {
        fprintf(stderr, "Error: Max allowed threads is %d\n", MAX_THREADS);
        exit(1);
    }

    // Creates thread objects that do not yet represent a thread.
    std::thread workers[MAX_THREADS];
    WorkerArgs args[MAX_THREADS];

    for (int i=0; i<numThreads; i++) {
      
        // TODO FOR CS149 STUDENTS: You may or may not wish to modify
        // the per-thread arguments here.  The code below copies the
        // same arguments for each thread
        args[i].x0 = x0;
        args[i].y0 = y0;
        args[i].x1 = x1;
        args[i].y1 = y1;
        args[i].width = width;
        args[i].height = height;
        args[i].maxIterations = maxIterations;
        args[i].numThreads = numThreads;
        args[i].output = output;
      
        args[i].threadId = i;
    }

    // Spawn the worker threads.  Note that only numThreads-1 std::threads
    // are created and the main application thread is used as a worker
    // as well.
    for (int i=1; i<numThreads; i++) {
        workers[i] = std::thread(workerThreadStart, &args[i]);
    }
    
    workerThreadStart(&args[0]);

    // join worker threads
    double startTime = CycleTimer::currentSeconds();

    for (int i=1; i<numThreads; i++) {
        workers[i].join();
    }

    double endTime = CycleTimer::currentSeconds();
    printf("All threads joined. Total time for joining: %.3f ms\n", (endTime - startTime) * 1000);
}

