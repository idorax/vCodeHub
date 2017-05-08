/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

/**
 * NOTE: It took me almost 5 hours to finish the implementation for the
 *       exercise in the following by using generic singly-linked list.
 *       And it is full of fun as I enjoy C programming :-)
 */

/**
 *#
 *# Copyright (c) 2011, 2016, Oracle and/or its affiliates. All rights reserved.
 *#
 *
 * C Programming Test II
 * =====================
 *
 * Candidate's Name: [                           ]
 * Time taken to complete this exercise: [       ]
 *
 * ============
 * INSTRUCTIONS
 * ============
 *
 * 1. Take up to 3 hours.
 *
 *    This is a timed exercise and the goal is to have a working program
 *    by the end. It is suggested that you first implement any mandatory
 *    options and then implement any additional options.
 *
 * 2. If you have questions please ask.
 *
 *    If you are doing the assignment at home and can't ask questions
 *    please interpret the instructions as best you can and make note of
 *    what assumption(s) you made, why, and include these notes with your
 *    completed assignment.
 *
 * 3. You may use man pages but no other references.
 *
 * 4. Please implement exactly what is specified (do not improvise or add
 *    features that are not spelled out explicitly below).
 *
 *    Additionally, please use standard C and POSIX for implementation of
 *    your program (if you are implementing this on a Linux box be sure
 *    not to use any non-standard, non-POSIX interfaces).
 *
 *    The program work should not be offloaded to any sub-process or shell
 *    (do not use exec(), system(), or any similar call which invokes a
 *    new process).
 *
 * 5. It should be possible to compile the program using a simple command
 *    line invocation of the compiler, e.g.
 *
 *    $ gcc -Wall -o foo foo.c
 *
 *    Please be sure to note the compilation command for your program when
 *    you submit your completed assignment.
 *
 * ==========
 * ASSIGNMENT
 * ==========
 *
 * Write a C program with the following command line interface:
 *
 *     foo [-h] [-aflm] [-d dir] [-s sid] [-c cid] [-o filename]
 *
 * foo will take input from a set of files in a specific folder about
 * student names, courses and scores and create a report for that
 * data listing statistics for each course and each student in the
 * average, highest and lowest scores, or a list of the failed results
 * (score < 60) with the course ids, student ids and scores.
 *
 * Take the following data as the input examples, notes:
 *
 * 1) The separator of data items can be a tab or a write space.
 * 2) You need to take care of the larger data set than that in this
 *    example.
 * 3) Student ids and course ids are not necessarily sequential numbers.
 *
 *     students.dat
 *
 *         #sid            #name
 *         1               John
 *         2               Smith
 *         3               Tom
 *         4               Amy
 *         5               Alice
 *
 *     courses.dat
 *
 *         #name           #cid
 *         math            4
 *         english         1
 *         art             2
 *
 *     scores.dat
 *
 *         #cid    #sid    #score
 *         1       1       80
 *         1       2       85
 *         1       3       90
 *         1       4       85
 *         1       5       50
 *         2       1       50
 *         2       2       85
 *         2       3       90
 *         2       4       85
 *         2       5       50
 *         4       1       80
 *         4       2       85
 *         4       3       90
 *         4       4       85
 *         4       5       80
 *
 * If the "-h" option is specified or if there are any errors in the
 * command line foo prints a usage message (along with a more specific
 * message about the exact nature of the problem). For instance, if there
 * aren't enough command line arguments or an option is specified that you
 * haven't implemented, the usage is printed along with a specific error
 * message.
 *
 * Implementing the "-h" and the following options are mandatory:
 *
 *     a) -a : Only report average scores.
 *     b) -m : Only report highest scores.
 *     c) -l : Only report lowest scores.
 *
 * Implementing any of the following options is optional and will carry
 * additional credits:
 *
 *     a) -f       : Report failed courses.
 *     b) -s [sid] : Specify the student id to report.
 *     c) -c [cid] : Specify the course id to report.
 *     d) -d [dir] : Use folder dir for the files as the input to the program.
 *     e) -o [filename] : Output the result data to the specified file.
 *
 * Note: You need to take care of the combinations of options, for example,
 *       when both -a and -s are specified, print the average score(s) of the
 *       particular student.
 *
 * The section below on USAGE EXAMPLES contains examples of the expected
 * output from these various command line options. Please note that the
 * examples given below are illustrative and may not represent every
 * potential type of input value that might legally be used.
 *
 * ==============
 * USAGE EXAMPLES
 * ==============
 *
 * 1.      User requests the usage statement.
 *
 *         $ ./foo -h
 *         Usage: ./foo [-h] [-aflm] [-d dir] [-s sid] [-c cid] [-o filename]
 *            -h            Show the help message.
 *            -a            Only report the average scores.
 *            -m            Only report the highest scores.
 *            -l            Only report the  lowest scores.
 *            -f            Only report on tests which failed.
 *            -s [sid]      Only report on student id specified.
 *            -c [cid]      Only report on course  id specified.
 *            -d [dir]      Use dir to get input files.
 *            -o [filename] Output the summary data to the specified file.
 *
 *         Hint: You can use the C function getopt() to handle options. If you
 *               are not familiar with getopt(), use the man page for help.
 *
 *               man -s3 getopt
 *
 * 2.      User specified "-x" on the command line, which is an invalid option.
 *
 *         $ ./foo -x
 *         ./foo: -x invalid option
 *         Usage: ./foo [-h] [-aflm] [-d dir] [-s sid] [-c cid] [-o filename]
 *
 * 3.      Do a full statistics on all courses and students.
 *
 *         foo works in the current work directory and the result looks like:
 *
 *         $ ./foo
 *         ------- Courses summary -------
 *         rank    cid     course  sum     highest lowest  average
 *         =========================================================
 *         1       4       math    420     90      80      84.0
 *         2       1       english 390     90      50      78.0
 *         3       2       art     360     90      50      72.0
 *
 *         ------- Students summary -------
 *         rank    sid     name    sum     highest lowest  average
 *         =========================================================
 *         1       3       Tom     270     90      90      90.0
 *         2       2       Smith   255     85      85      85.0
 *         3       4       Amy     255     85      85      85.0
 *         4       1       John    210     80      50      70.0
 *         5       5       Alice   180     80      50      60.0
 *
 * 4.      Report average scores for each course and each student:
 *
 *         $ ./foo -a
 *         ------- Courses summary -------
 *         cid     course  average
 *         =========================================
 *         4       math    84.0
 *         1       english 78.0
 *         2       art     72.0
 *
 *         ------- Students summary -------
 *         sid     name    average
 *         =========================================
 *         3       Tom     90.0
 *         2       Smith   85.0
 *         4       Amy     85.0
 *         1       John    70.0
 *         5       Alice   60.0
 *
 * 5.      Report highest and lowest scores.
 *
 *         $ ./foo -m
 *         ------- Courses summary -------
 *         cid     course  highest
 *         =========================================
 *         4       math    90
 *         1       english 90
 *         2       art     90
 *
 *         ------- Students summary -------
 *         sid     name    highest
 *         =========================================
 *         3       Tom     90
 *         2       Smith   85
 *         4       Amy     85
 *         1       John    80
 *         5       Alice   80
 *
 *         $ ./foo -l
 *         ------- Courses summary -------
 *         cid     course  lowest
 *         =========================================
 *         4       math    80
 *         1       english 50
 *         2       art     50
 *
 *         ------- Students summary -------
 *         sid     name    lowest
 *         =========================================
 *         3       Tom     90
 *         2       Smith   85
 *         4       Amy     85
 *         1       John    50
 *         5       Alice   50
 *
 * 6.      Report failed tests.
 *
 *         $ ./foo -f
 *         ------- Failure summary -------
 *         course  sid     name
 *         =========================================
 *         math    --      ----
 *         english 5       Alice
 *         art     1       John
 *                 5       Alice
 *
 * 7.      Report on a particular course.
 *
 *         $ ./foo -c 1
 *         ------- Course summary -------
 *         cid     course  highest lowest  average
 *         =========================================
 *         1       english 90      50      78.0
 *
 *         ------- Failure summary -------
 *         course  sid     name
 *         =========================================
 *         english 5       Alice
 *
 * 8.      Report on a particular student.
 *
 *         $ ./foo -s 5
 *         ------- Student summary -------
 *         id      name    highest lowest  average
 *         =========================================
 *         5       Alice   80      50      60.0
 *
 *         ------- Failure summary -------
 *         course  score
 *         =========================================
 *         english 50
 *         art     50
 *
 * 9.      Specify the input data folder
 *
 *         $ ./foo -d /tmp/data
 *
 *         [Output example omitted.]
 *
 * 10.     User enters data via a file and requested the output be written to
 *         a file using the "-o" option.
 *
 *         $ ./foo -o /tmp/foo.out
 *
 *         [Contents of /tmp/foo.out omitted.]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "list.h"
#include "foo.h"

static void
do_foo_0(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view)
{
	uint16_t i = 0;

	printf("------- Courses summary -------\n");
	printf("rank\tcid\tcourse\tsum\thighest\tlowest\taverage\n");
	printf("=========================================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		i++;

		uint8_t	max = 0;
		uint8_t	min = ~0;
		uint32_t sum = 0;
		uint32_t nstu = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((ss_t *)list_l2d(q))->score;
			max = MAX(max, score);
			min = MIN(min, score);
			sum += score;
			nstu++;
		}

		char *name = get_course_name(cid, l_cou);
		printf("%u\t%u\t%s\t%u\t%d\t%d\t%.1f\n",
		       i, cid, name, sum, max, min, sum / 1.0 / nstu);
	}

	i = 0;
	printf("\n------- Students summary -------\n");
	printf("rank\tsid\tname\tsum\thighest\tlowest\taverage\n");
	printf("=========================================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		i++;

		uint8_t	max = 0;
		uint8_t min = ~0;
		uint32_t sum = 0;
		uint32_t ncou = 0;
		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((cs_t *)list_l2d(q))->score;
			max = MAX(max, score);
			min = MIN(min, score);
			sum += score;
			ncou++;
		}

		char *name = get_student_name(sid, l_stu);
		printf("%u\t%u\t%s\t%u\t%u\t%u\t%.1f\n",
		       i, sid, name, sum, max, min, sum / 1.0 / ncou);
	}
}

static void
do_foo_a(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t xcid, uint16_t xsid)
{
	printf("------- Courses summary -------\n");
	printf("cid\tcourse\taverage\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (xcid != 0 && cid != xcid)
			continue;

		uint32_t sum  = 0;
		uint32_t ncou = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			sum += ((ss_t *)list_l2d(q))->score;
			ncou++;
		}

		char *name = get_course_name(cid, l_cou);
		printf("%u\t%s\t%.1f\n", cid, name, sum / 1.0 / ncou);
	}

	printf("\n------- Students summary -------\n");
	printf("sid\tname\taverage\n");
	printf("=========================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (xsid != 0 && sid != xsid)
			continue;

		uint32_t sum  = 0;
		uint32_t ncou = 0;
		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			sum += ((cs_t *)list_l2d(q))->score;
			ncou++;
		}

		char *name = get_student_name(sid, l_stu);
		printf("%u\t%s\t%.1f\n", sid, name, sum / 1.0 / ncou);
	}
}

static void
do_foo_m(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t xcid, uint16_t xsid)
{
	printf("------- Courses summary -------\n");
	printf("cid\tcourse\thighest\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (xcid != 0 && cid != xcid)
			continue;

		uint8_t	max = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((ss_t *)list_l2d(q))->score;
			max = MAX(max, score);
		}

		char *name = get_course_name(cid, l_cou);
		printf("%u\t%s\t%u\n", cid, name, max);
	}

	printf("\n------- Students summary -------\n");
	printf("sid\tname\thighest\n");
	printf("=========================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (xsid != 0 && sid != xsid)
			continue;

		uint8_t	max = 0;
		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((cs_t *)list_l2d(q))->score;
			max = MAX(max, score);
		}

		char *name = get_student_name(sid, l_stu);
		printf("%u\t%s\t%u\n", sid, name, max);
	}
}

static void
do_foo_l(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t xcid, uint16_t xsid)
{
	printf("------- Courses summary -------\n");
	printf("cid\tcourse\tlowest\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (xcid != 0 && cid != xcid)
			continue;

		uint8_t	min = ~0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((ss_t *)list_l2d(q))->score;
			min = MIN(min, score);
		}

		char *name = get_course_name(cid, l_cou);
		printf("%u\t%s\t%u\n", cid, name, min);
	}

	printf("\n------- Students summary -------\n");
	printf("sid\tname\tlowest\n");
	printf("=========================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (xsid != 0 && sid != xsid)
			continue;

		uint8_t	min = ~0;
		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((cs_t *)list_l2d(q))->score;
			min = MIN(min, score);
		}

		char *name = get_student_name(sid, l_stu);
		printf("%u\t%s\t%u\n", sid, name, min);
	}
}

static void
do_foo_f(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t xcid, uint16_t xsid)
{
	printf("------- Failure summary -------\n");
	printf("course\tsid\tname\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (xcid != 0 && cid != xcid)
			continue;

		char *cname = get_course_name(cid, l_cou);

		uint8_t flag = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			ss_t *pss = list_l2d(q);
			uint16_t sid = pss->sid;
			if (xsid != 0 && sid != xsid)
				continue;

			if (((ss_t *)list_l2d(q))->score < 60) {
				if (flag++ != 0)
					cname = "";

				char *sname = get_student_name(sid, l_stu);
				printf("%s\t%u\t%s\n", cname, sid, sname);
			}

		}

		if (flag == 0)
			printf("%s\t--\t----\n", cname);
	}
}

static void
do_foo_c(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t id)
{
	uint8_t flag = 0;
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (id == cid) {
			flag++;
			break;
		}
	}
	if (flag == 0) {
		fprintf(stderr, "Oops, cid %u not found!\n", id);
		return;
	}

	uint8_t	max = 0;
	uint8_t min = ~0;
	printf("------- Course summary -------\n");
	printf("cid\tcourse\thighest\tlowest\taverage\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (id != cid)
			continue;

		uint32_t sum  = 0;
		uint32_t ncou = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((ss_t *)list_l2d(q))->score;
			max = MAX(max, score);
			min = MIN(min, score);
			sum += score;
			ncou++;
		}

		char *name = get_course_name(cid, l_cou);
		printf("%u\t%s\t%d\t%d\t%.1f\n",
		       cid, name, max, min, sum / 1.0 / ncou);
	}

	if (min >= 60) {
		printf("\nGood, nobody fails\n");
		return;
	}

	printf("\n------- Failure summary -------\n");
	printf("course\tsid\tname\n");
	printf("=========================================\n");
	for (list_t *p = l_cou_view; p != NULL; p = p->next) {
		course_view_t *pcou = list_l2d(p);
		uint16_t cid = pcou->cid;
		if (id != cid)
			continue;

		char *cname = get_course_name(cid, l_cou);

		uint8_t flag = 0;
		list_t *l = list_d2l(pcou->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			ss_t *pss = list_l2d(q);

			if (pss->score < 60) {
				if (flag++ != 0)
					cname = "";

				char *sname = get_student_name(pss->sid, l_stu);
				printf("%s\t%u\t%s\n", cname, pss->sid, sname);
			}

		}

		if (flag == 0)
			printf("%s\t--\t----\n", cname);
	}
}

static void
do_foo_s(list_t *l_stu, list_t *l_stu_view,
         list_t *l_cou, list_t *l_cou_view,
	 uint16_t id)
{
	uint8_t flag = 0;
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (id == sid) {
			flag++;
			break;
		}
	}
	if (flag == 0) {
		fprintf(stderr, "Oops, sid %u not found!\n", id);
		return;
	}

	uint8_t	max = 0;
	uint8_t min = ~0;
	printf("------- Student summary -------\n");
	printf("id\tname\thighest\tlowest\taverage\n");
	printf("=========================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (id != sid)
			continue;

		uint32_t sum  = 0;
		uint32_t ncou = 0;
		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			uint8_t score = ((cs_t *)list_l2d(q))->score;
			max = MAX(max, score);
			min = MIN(min, score);
			sum += score;
			ncou++;
		}

		char *name = get_student_name(sid, l_stu);
		printf("%u\t%s\t%d\t%d\t%.1f\n",
		       sid, name, max, min, sum / 1.0 / ncou);
	}

	if (min >= 60) {
		printf("\nGood, no failure course\n");
		return;
	}

	printf("\n------- Failure summary -------\n");
	printf("course\tscore\n");
	printf("=========================================\n");
	for (list_t *p = l_stu_view; p != NULL; p = p->next) {
		student_view_t *pstu = list_l2d(p);
		uint16_t sid = pstu->sid;
		if (id != sid)
			continue;

		list_t *l = list_d2l(pstu->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			cs_t *pcs = list_l2d(q);

			if (pcs->score < 60) {
				char *cname = get_course_name(pcs->cid, l_cou);
				printf("%s\t%u\n", cname, pcs->score);
			}
		}
	}
}

static void
usage(char *s, boolean_t full_usage)
{
	fprintf(stderr, "Usage: %s [-h] [-aflm] "
	        "[-d dir] [-s sid] [-c cid] [-o filename]\n", s);
	if (!full_usage)
		return;
	fprintf(stderr, "   -h\t\t Show the help message.\n");
	fprintf(stderr, "   -a\t\t Only report the average scores.\n");
	fprintf(stderr, "   -m\t\t Only report the highest scores.\n");
	fprintf(stderr, "   -l\t\t Only report the  lowest scores.\n");
	fprintf(stderr, "   -f\t\t Only report on tests which failed.\n");
	fprintf(stderr, "   -s [sid]\t Only report on student id specified.\n");
	fprintf(stderr, "   -c [cid]\t Only report on course  id specified.\n");
	fprintf(stderr, "   -d [dir]\t Use dir to get input files.\n");
	fprintf(stderr, "   -o [filename] Output the summary data to"
	        " the specified file.\n");
}

int
main(int argc, char *argv[])
{
	int rc = 0;

	list_t *l_stu = NULL; /* list of students */
	list_t *l_cou = NULL; /* list of courses  */
	list_t *l_sco = NULL; /* list of scores   */
	list_t *l_stu_view = NULL; /* list of student view (student_view_t) */
	list_t *l_cou_view = NULL; /* list of course  view (course_view_t) */

#define FLAG_A	(1<<0) /* Average    */
#define FLAG_F	(1<<1) /* Failed     */
#define FLAG_L	(1<<2) /* Lowest     */
#define FLAG_M	(1<<3) /* Highest    */
#define FLAG_S	(1<<4) /* Student ID */
#define FLAG_C	(1<<5) /* Course  ID */
	char flag;
	char *data_dirname = D_DATA;
	char data_file[256];
	int sid = 0;
	int cid = 0;
	char *outfile = NULL;
	int fd = -1;
	char c;
	while ((c = getopt(argc, argv, ":aflmd:s:c:o:hD")) != -1) {
		switch(c) {
		case 'a':
			flag = FLAG_A;
			break;

		case 'f':
			flag = FLAG_F;
			break;

		case 'l':
			flag = FLAG_L;
			break;

		case 'm':
			flag = FLAG_M;
			break;

		case 'd':
			data_dirname = optarg;
			break;

		case 's':
			flag |= FLAG_S;
			sid = atoi(optarg);
			break;

		case 'c':
			flag |= FLAG_C;
			cid = atoi(optarg);
			break;

		case 'o':
			outfile = optarg;
			break;

		case 'h':
			usage(argv[0], B_TRUE);
			return -1;

		case 'D': /* private option for debugging only */
			debug = B_TRUE;
			break;

		case ':':
			fprintf(stderr, "Option '-%c' wants an argument\n",
			        optopt);
			return -1;

		case '?':
			fprintf(stderr, "%s: -%c invalid option\n",
			        argv[0], optopt);
			usage(argv[0], B_FALSE);
			return -1;
		}
	}

	argc -= optind;
	argv += optind;

	memset(data_file, 0, sizeof (data_file));
	snprintf(data_file, sizeof (data_file), "%s/%s",
	         data_dirname, F_DATA_STUDENTS);
	rc = init_student_list(&l_stu, data_file);
	if (rc != 0) {
		rc = -1;
		goto done1;
	}
	DEBUG(printf("\n"));

	memset(data_file, 0, sizeof (data_file));
	snprintf(data_file, sizeof (data_file), "%s/%s",
	         data_dirname, F_DATA_COURSES);
	rc = init_course_list(&l_cou, data_file);
	if (rc != 0) {
		rc = -2;
		goto done2;
	}
	DEBUG(printf("\n"));

	memset(data_file, 0, sizeof (data_file));
	snprintf(data_file, sizeof (data_file), "%s/%s",
	         data_dirname, F_DATA_SCORES);
	rc = init_score_list(&l_sco, data_file);
	if (rc != 0) {
		rc = -3;
		goto done3;
	}
	DEBUG(printf("\n"));

	rc = init_student_view(l_stu, l_sco, &l_stu_view);
	if (rc != 0) {
		rc = -4;
		goto done4;
	}
	DEBUG(printf("\n"));

	rc = init_course_view(l_cou, l_sco, &l_cou_view);
	if (rc != 0) {
		rc = -5;
		goto done5;
	}
	DEBUG(printf("\n"));

	if (outfile != NULL) {
		fd = open(outfile, O_WRONLY|O_CREAT|O_TRUNC, 0644);
		if (fd == -1) {
			fprintf(stderr, "fail to open file %s:%s\n",
			        outfile, strerror(errno));
			rc = -6;
			goto done5;
		}

		if (dup2(fd, 1) == -1) {
			fprintf(stderr, "fail to dup2(%d, 1):%s\n",
			        fd, strerror(errno));
			rc = -7;
			goto done5;
		}
	}

	DEBUG(printf("**************************************************\n\n"));
	if (flag & FLAG_A)
		do_foo_a(l_stu, l_stu_view, l_cou, l_cou_view, cid, sid);
	else if (flag & FLAG_F)
		do_foo_f(l_stu, l_stu_view, l_cou, l_cou_view, cid, sid);
	else if (flag & FLAG_M)
		do_foo_m(l_stu, l_stu_view, l_cou, l_cou_view, cid, sid);
	else if (flag & FLAG_L)
		do_foo_l(l_stu, l_stu_view, l_cou, l_cou_view, cid, sid);
	else if (flag & FLAG_C)
		do_foo_c(l_stu, l_stu_view, l_cou, l_cou_view, cid);
	else if (flag & FLAG_S)
		do_foo_s(l_stu, l_stu_view, l_cou, l_cou_view, sid);
	else
		do_foo_0(l_stu, l_stu_view, l_cou, l_cou_view);
	DEBUG(printf("**************************************************\n\n"));

done5:
	fini_course_view(l_cou_view);	DEBUG(printf("\n"));
done4:
	fini_student_view(l_stu_view);	DEBUG(printf("\n"));
done3:
	fini_score_list(l_sco);		DEBUG(printf("\n"));
done2:
	fini_course_list(l_cou);	DEBUG(printf("\n"));
done1:
	fini_student_list(l_stu);	DEBUG(printf("\n"));

	if (fd != -1)
		close(fd);

	return rc;
}
