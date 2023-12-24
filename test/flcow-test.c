/*    Copyright 2023 Davide Libenzi
 * 
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 * 
 *        http://www.apache.org/licenses/LICENSE-2.0
 * 
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 * 
 */


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>



#define DUMMY_DATA1 "FLCOW Test Data Before"
#define DUMMY_DATA2 "FLCOW Test Data After"


static int create_file(int (*ofunc)(char const *, int, ...),
		       char const *name, char const *data) {
	int fd;

	if ((fd = (*ofunc)(name, O_WRONLY | O_CREAT, 0666)) == -1) {
		perror(name);
		return -1;
	}
	write(fd, data, strlen(data));
	close(fd);

	return 0;
}


static int test_flcow(int (*ofunc)(char const *, int, ...), char const *ext) {
	struct stat stb1, stb2;
	char buf[512], name1[512], name2[512];

	getcwd(buf, sizeof(buf) - 1);
	setenv("FLCOW_PATH", buf, 1);

	sprintf(name1, ",,flcow-test1++.%u", getpid());
	if (create_file(ofunc, name1, DUMMY_DATA1) < 0) {
		fprintf(stdout, "[%s] Test Result\t\t[ FAILED ]\n", ext);
		return 1;
	}
	fprintf(stdout, "[%s] File Creation\t\t[ OK ]\n", ext);

	sprintf(name2, ",,flcow-test2++.%u", getpid());
	if (link(name1, name2) < 0) {
		unlink(name1);
		fprintf(stdout, "[%s] Test Result\t\t[ FAILED ]\n", ext);
		return 2;
	}
	fprintf(stdout, "[%s] Link Creation\t\t[ OK ]\n", ext);

	stat(name1, &stb1);
	stat(name2, &stb2);
	if (stb1.st_nlink < 2 || stb2.st_nlink < 2) {
		unlink(name2);
		unlink(name1);
		fprintf(stdout, "[%s] Link Check\t\t[ FAILED ]\n", ext);
		return 3;
	}
	fprintf(stdout, "[%s] Link Check\t\t[ OK ]\n", ext);

	if (create_file(ofunc, name1, DUMMY_DATA2) < 0) {
		unlink(name2);
		unlink(name1);
		fprintf(stdout, "[%s] File Rewrite\t\t[ FAILED ]\n", ext);
		return 4;
	}
	fprintf(stdout, "[%s] File Rewrite\t\t[ OK ]\n", ext);

	stat(name1, &stb1);
	stat(name2, &stb2);
	if (stb1.st_nlink > 1 || stb2.st_nlink > 1) {
		unlink(name2);
		unlink(name1);
		fprintf(stdout, "[%s] COW Check\t\t[ FAILED ]\n", ext);
		return 5;
	}
	fprintf(stdout, "[%s] COW Check\t\t[ OK ]\n", ext);

	unlink(name2);
	unlink(name1);

	fprintf(stdout, "[%s] Test Result\t\t[ OK ]\n\n", ext);

	return 0;
}


int main(int argc, char **argv) {
	int res;

	if ((res = test_flcow(open, "open")) != 0)
		return res;
#ifdef HAVE_OPEN64
	if ((res = test_flcow(open64, "open64")) != 0)
		return res;
#endif

	return 0;
}

