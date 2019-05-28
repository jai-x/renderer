#include <stdio.h>
#include <stdlib.h>

#include "buffer.h"


int
main(void)
{
	buffer img = buffer_new(1000, 1000);

	color blue   = {  0,   0, 255};
	color green  = {  0, 255,   0};
	color red    = {255,   0,   0};
	color yellow = {255, 255,   0};
	color white  = {255, 255, 255};

	buffer_line(img, 499, 0, 499, 999, blue);
	buffer_line(img, 0, 499, 999, 499, blue);

	buffer_line(img, 0, 0, 999, 499, red);
	buffer_line(img, 999, 599, 0, 99, green);

	buffer_line(img, 0, 499, 999, 0, yellow);
	buffer_line(img, 999, 99, 0, 599, white);

	buffer_line(img, 0, 0, 499, 999, red);
	buffer_line(img, 599, 999, 99, 0, green);

	buffer_line(img, 499, 999, 999, 0, yellow);
	buffer_line(img, 899, 0, 399, 999, white);

	buffer_write(img, "output.ppm");
	buffer_free(img);

	return EXIT_SUCCESS;
}
