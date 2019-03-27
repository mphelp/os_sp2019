#pragma once

/* Defines */
#define MAX_CHARACTER_INPUT 1000
#define MAX_DISTINCT_WORDS 	100

// Courtesy of Prof. Bui's Systems Programming course:
#define debug(M, ...)\
	fprintf(stderr, "%s:%d:%s" M "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__)
#define errInput(M, ...)\
	fprintf(stderr, "%s: " M "\n", __FILE__, ##__VA_ARGS__)
#define errInput2(M, ...)\
	fprintf(stderr, "ERROR: " M "\n", ##__VA_ARGS__)
#define streq(s0, s1) (strcmp((s0), (s1)) == 0)

