/**
 * Copyright (C) 2008-2009 by Simon Schönfeld
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 **/

#define _GNU_SOURCE
#include <unistd.h>
#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>

#define MAJOR_VERSION 0
#define MINOR_VERSION 9

void print_usage()
{
	printf(
		"Usage: %s [OPTIONS] EXECUTABLE [PROGOPTIONS]\n"
		"Calls EXECUTABLE with the arguments PROGOPTIONS and quits. This will cause init\n"
		"to adopt the children and unbind the program from the calling terminal.\n"
		"\n"
		"Options:\n"
		"  -n, --noredir  prevent the program's stdin and stderr to be redirected\n"
		"                 to /dev/null\n"
		"  -h, --help     display this help and exit\n"
		"  -v  --version  output version information and exit\n"
		"\n"
		"Exit status is 0 if OK, 1 if the commandline arguments are invalid, 2 if\n"
		"the process failed to fork, 3 if EXECUTABLE couldn't be called\n"
		"\n"
		"Report bugs to <simon.schoenfeld@web.de>\n"
		, program_invocation_name);
}

void print_version()
{
	printf(
		"fork %i.%i\n"
		"Copyright (C) 2009-2009 Simon Schönfeld.\n"
		"This is free software.  You may redistribute copies of it under the terms of\n"
		"the GNU General Public License <http://www.gnu.org/licenses/gpl.html>.\n"
		"There is NO WARRANTY, to the extent permitted by law.\n"
		"\n"
		"Written by Simon Schönfeld\n"
		, MAJOR_VERSION, MINOR_VERSION);
}

int main(int argc, char** argv)
{
	int redir = 1;

	if(argc < 2) /* We need at least one argument */
	{
		print_usage();
		return 1;
	}

	/* -h or --help in the commandline-options? */
	/* Only check if it's in the second field, otherwise calls like this
	would behave strange: "fork ls --help" */
	if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)
	{
		print_usage();
		return 0;
	}

	/* The same with -v and --version */
	if(strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--version") == 0)
	{
		print_version();
		return 0;
	}

	/* And the redir-stuff */
	if(strcmp(argv[1], "-n") == 0 || strcmp(argv[1], "--noredir") == 0)
	{
		redir = 0;
		argc--;
		argv++;
	}

	pid_t pid = 0;
	pid = fork();
	
	if(pid == -1) /* Means an error occured */
		error(2, errno, "Couldn't fork");

	if(pid != 0) /* Are we the parent-process? */
		return 0; /* We are, so we quit execution */

	/* Create the argc/argv - we just strip the first field, since
	this is the call of this program, everything else is passed
	unchanged to the exec-call */
	int e_argc = argc - 1;
	char* e_argv[e_argc + 1];

	int i;
	for(i = 0; i < e_argc; i++)
		e_argv[i] = argv[i + 1];
	
	e_argv[e_argc] = NULL; /* the last entry has to be NULL */

	/* redirect stdour/stderr */
	if(redir)
	{
		freopen("/dev/null", "w", stdout);
		freopen("/dev/null", "w", stderr);
	}

	if(execvp(e_argv[0], e_argv) == -1)
		error(3, errno, "Couldn't execute %s", e_argv[0]);
	
	return 0; /* Never reached */
}

