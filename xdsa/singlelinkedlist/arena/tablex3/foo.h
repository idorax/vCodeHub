/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#ifndef	_FOO_H
#define	_FOO_H

#include "list.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define D_DATA		"./data"
#define F_DATA_STUDENTS	"students.dat"
#define F_DATA_COURSES	"courses.dat"
#define F_DATA_SCORES	"scores.dat"
#define MAX_LINE_LEN	64 /* max width of per line in data files */

#define MAX(a, b)	(((a) > (b)) ? (a) : (b))
#define MIN(a, b)	(((a) < (b)) ? (a) : (b))

typedef unsigned char		uint8_t;
typedef unsigned short		uint16_t;
typedef unsigned int		uint32_t;
typedef unsigned long long	uint64_t;

typedef enum {B_FALSE, B_TRUE} boolean_t;

typedef struct student_s {
	uint16_t	sid;
	char		name[32];
	list_t		link;
} student_t;

typedef struct course_s {
	uint16_t	cid;
	char		name[32];
	list_t		link;
} course_t;

typedef struct score_s {
	uint16_t	sid;
	uint16_t	cid;
	uint8_t		score;
	list_t		link;
} score_t;

typedef struct cs_s {
	uint16_t	cid;
	uint8_t		score;
	list_t		link;
} cs_t;

typedef struct ss_s {
	uint16_t	sid;
	uint8_t		score;
	list_t		link;
} ss_t;

typedef struct student_view_s {
	uint32_t	sid;	/* student id */
	cs_t		*lcs;	/* list of struct cs_s */
	list_t		link;
} student_view_t;

typedef struct course_view_s {
	uint16_t	cid;	/* course id */
	ss_t		*lss;	/* list of struct ss_s */
	list_t		link;
} course_view_t;

typedef struct sort_view_s {
	uint16_t	id;
	uint32_t	sum;
} sort_view_t;

extern boolean_t debug;
#define DEBUG(a) { if (debug) a; }

extern  int init_student_list(list_t **l, char *file);
extern void fini_student_list(list_t *l);
extern  int init_course_list(list_t **l, char *file);
extern void fini_course_list(list_t *l);
extern  int init_score_list(list_t **l, char *file);
extern void fini_score_list(list_t *l);

extern  int init_student_view(list_t *lstu, list_t *lsco, list_t **lview);
extern void fini_student_view(list_t *lview);
extern  int init_course_view(list_t *lcou, list_t *lsco, list_t **lview);
extern void fini_course_view(list_t *lview);

extern char *get_student_name(uint32_t id, list_t *l);
extern char *get_course_name(uint32_t id, list_t *l);

#ifdef	__cplusplus
}
#endif

#endif	/* _FOO_H */
