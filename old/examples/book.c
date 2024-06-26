#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "du.h"

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

#define _WORD_N 32

/************************************************************************************************************/
/************************************************************************************************************/
/************************************************************************************************************/

int
main(int argc, char **argv)
{
	du_book_t book;
	char *s;

	size_t n       = argc > 1 ? strtoul(argv[1], NULL, 0) : 10;
	size_t n_alloc = argc > 2 ? strtoul(argv[2], NULL, 0) : n;

	/* setup */

	du_book_init(&book, n_alloc * 2, _WORD_N);

	/* write */

	for (size_t i = 0; i < n; i++) {
		s = du_book_get_new_word(&book, i % 5 == 0);
		sprintf(s, "word_%li", i);
	}

	/* read */

/*
	s = NULL;
	while (du_book_get_next_word(&book, &s)) {
		// printf("%s\n", s);
	}
*/

	for (size_t i = 0; i < book.n_groups; i++) {
		for (size_t j = 0; j < du_book_get_group_length(&book, i); j++) {
			printf("%s\t", du_book_get_word_in_group(&book, i, j));
		}
		printf("\n");
	}

	printf(
		"\nwords\t%li\t%li\ngroups\t%li\t%li\n",
		book.n_words, book.n_alloc, book.n_groups, book.n_alloc);

	for (size_t i = 0; i < book.n_groups; i++) {
		printf("g%li\t%li\t%li\n", i, book.groups[i], du_book_get_group_length(&book, i));
	}

	/* end */

	du_book_reset(&book);

	return 0;
}
