#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>


int _INITIAL_CAPACITY_ = 20;

struct vector { // simple vector implementation. array capacity is doubled in the event of size reaching capacity
	size_t capacity;
	size_t size;

	char* arr;

	void (*append)(struct vector*, char); // puts element in the first unused index
	void (*dealloc)(struct vector*); // frees the memory taken by the array
	char* (*copy_arr)(struct vector*); // returns a pointer to the current content of the array reallocing to the minimum size before that, finishes with an empty array 
};

typedef struct vector vector; // typedef so that we dont have to write "struct vector" every time


void append(vector* this, char el){ // adds the char to the end of the array. takes care for cases when the allocated memory isn't enough
	 
	if ( this->arr == NULL ){ // the very first append for the vector goes here
		if ( ( this->arr = malloc(this->capacity) ) == NULL ) // just mallocs the given capcity
			err(1, "Couldn't malloc");
	}
	

	if ( this->size >= this->capacity ){ // check for size reaching capacity and doubles the capacity in this case
		this->capacity <<= 1;
		
		if ( ( this->arr = realloc(this->arr, this->capacity) ) == NULL ) // if realloc fails
			err(1, "Couldn't realloc");
	}
	
	this->arr[this->size++] = el; // the actuall adding of the char element
}

void dealloc(vector* this){ // frees the memory taken by the array and makes it point to NULL
	free(this->arr);
	this->arr = NULL;
	this->size = 0;
	this->capacity = _INITIAL_CAPACITY_;
	
}

char* copy_arr(vector* this){

	this->append(this, '\0'); // puts terminating \0 at the end of the array
	this->arr = realloc(this->arr, this->size); // shrinks the excess allocated bytes
	
	char* temp = this->arr; // holds a pointer to the current content of arr
	
	this->arr = NULL; // reinitializes array with NULL
	this->size = 0;
	this->capacity = _INITIAL_CAPACITY_;
	return temp;
}

void init(vector* this,size_t capacity){
	_INITIAL_CAPACITY_=capacity;
	this->capacity=capacity;
	this->size=0;
	this->arr=NULL;
	this->append=append;
	this->dealloc=dealloc;
	this->copy_arr=copy_arr;
}
#endif
