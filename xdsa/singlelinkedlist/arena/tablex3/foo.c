/*
 * Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "foo.h"

boolean_t debug = B_FALSE;

typedef struct sort_view_s {
	uint16_t	id;
	uint32_t	sum;
} sort_view_t;

static void
trim(char *dst, char *src)
{
	char *p = dst;
	char *q = src;

	while (*q != '\0') {
		if (*q != ' ' && *q != '\t')
			break;
		q++;
	}

	while (*q != '\0') {
		*p++ = *q++;
	}
	*p = '\0';

	p--;
	while (1) {
		if (*p != ' ' && *p != '\t' && *p != '\n' && *p != '\r')
			break;
		*p-- = '\0';
	}
}

static void
bubble_sort(sort_view_t *a, uint16_t n)
{
	for (uint16_t i = 0; i < n - 1; i++) {
		for (uint16_t j = 0; j < n - 1 - i; j++) {
			if ((a+j+1)->sum > (a+j)->sum) {
				uint16_t id  = (a+j+1)->id;
				uint32_t sum = (a+j+1)->sum;
				(a+j+1)->id  = (a+j)->id;
				(a+j+1)->sum = (a+j)->sum;
				(a+j)->id    = id;
				(a+j)->sum   = sum;
			}
		}
	}
}

int
init_student_list(list_t **l, char *file)
{
	int rc = 0;
	char tmp[MAX_LINE_LEN] = { 0 };
	char buf[MAX_LINE_LEN] = { 0 };

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "failed to open file %s: %s\n",
		        file, strerror(errno));
		return (-1);
	}

	while (fgets(tmp, sizeof(tmp), fp) != 0) {
		trim(buf, tmp);
		if (buf[0] == '#' || buf[0] == '\0')
			continue;

		student_t *p = (student_t *)(malloc(sizeof(student_t)));
		if (p == NULL) {
			fprintf(stderr, "failed to malloc():%s\n",
			        strerror(errno));
			rc = -1;
			goto done;
		}

		(void) memset(p, 0, sizeof (student_t));
		uint32_t sid = 0;
		sscanf(buf, "%u %s", &sid, p->name);
		p->sid = (uint16_t)sid;

		DEBUG(printf("%d\t%s\t%p <--malloc\n", p->sid, p->name, p));

		list_insert_tail(l, p, offsetof(student_t, link));
	}

done:
	fclose(fp);
	return (rc);
}

int
init_course_list(list_t **l, char *file)
{
	int rc = 0;
	char tmp[MAX_LINE_LEN] = { 0 };
	char buf[MAX_LINE_LEN] = { 0 };

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "failed to open file %s: %s\n",
		        file, strerror(errno));
		return (-1);
	}

	while (fgets(tmp, sizeof(tmp), fp) != 0) {
		trim(buf, tmp);
		if (buf[0] == '#' || buf[0] == '\0')
			continue;

		course_t *p = (course_t *)(malloc(sizeof(course_t)));
		if (p == NULL) {
			fprintf(stderr, "failed to malloc():%s\n",
			        strerror(errno));
			rc = -1;
			goto done;
		}

		(void) memset(p, 0, sizeof (course_t));
		uint32_t cid = 0;
		sscanf(buf, "%s %u", p->name, &cid);
		p->cid = (uint16_t)cid;

		DEBUG(printf("%d\t%s\t%p <--malloc\n", p->cid, p->name, p));

		list_insert_tail(l, p, offsetof(course_t, link));
	}

done:
	fclose(fp);
	return (rc);
}

int
init_score_list(list_t **l, char *file)
{
	int rc = 0;
	char tmp[MAX_LINE_LEN] = { 0 };
	char buf[MAX_LINE_LEN] = { 0 };

	FILE *fp = fopen(file, "r");
	if (fp == NULL) {
		fprintf(stderr, "failed to open file %s: %s\n",
		        file, strerror(errno));
		return (-1);
	}

	while (fgets(tmp, sizeof(tmp), fp) != 0) {
		trim(buf, tmp);
		if (buf[0] == '#' || buf[0] == '\0')
			continue;

		score_t *p = (score_t *)(malloc(sizeof(score_t)));
		if (p == NULL) {
			fprintf(stderr, "failed to malloc():%s\n",
			        strerror(errno));
			rc = -1;
			goto done;
		}

		(void) memset(p, 0, sizeof (score_t));
		uint32_t cid, sid, score;
		sscanf(buf, "%u %u %u", &cid, &sid, &score);
		p->cid = (uint16_t)cid;
		p->sid = (uint16_t)sid;
		p->score = (uint8_t)score;

		DEBUG(printf("%d\t%d\t%d\t%p <--malloc\n",
		      p->cid, p->sid, p->score, p));

		list_insert_tail(l, p, offsetof(score_t, link));
	}

done:
	fclose(fp);
	return (rc);
}

void
fini_score_list(list_t *l)
{
	for (list_t *p = l; p != NULL; p = p->next) {
		score_t *q = list_l2d(p);

		DEBUG(printf("%d\t%d\t%d\t%p <--free\n",
		      q->cid, q->sid, q->score, q));
		free(q);
	}
}

void
fini_course_list(list_t *l)
{
	for (list_t *p = l; p != NULL; p = p->next) {
		course_t *q = list_l2d(p);

		DEBUG(printf("%d\t%s\t%p <--free\n",
		      q->cid, q->name, q));
		free(q);
	}
}

void
fini_student_list(list_t *l)
{
	for (list_t *p = l; p != NULL; p = p->next) {
		student_t *q = list_l2d(p);

		DEBUG(printf("%d\t%s\t%p <--free\n",
		      q->sid, q->name, q));
		free(q);
	}
}

int
init_student_view(list_t *lstu, list_t *lsco, list_t **lview)
{
	int rc = 0;

	/*
	 * 1. init student sort view array
	 */

	/* 1.1 get length of student list */
	uint16_t sv_sz = 0;
	for (list_t *p = lstu; p != NULL; p = p->next)
		sv_sz++;

	/* 1.2 malloc a new array */
	sort_view_t *sv = (sort_view_t *)malloc(sizeof (sort_view_t) * sv_sz);
	if (sv == NULL) {
		fprintf(stderr, "failed to malloc:%s\n", strerror(errno));
		return (-1);
	}
	memset(sv, 0, sizeof (sort_view_t) * sv_sz);
	DEBUG(printf("%p <-- MALLOC@student_sort_view\n", sv));

	/* 1.3 init the array */
	int i = 0;
	for (list_t *p = lstu; p != NULL; p = p->next) {
		student_t *pstu = list_l2d(p);

		uint16_t sid = pstu->sid;
		uint32_t sum = 0;

		for (list_t *q = lsco; q != NULL; q = q->next) {
			score_t *psco = list_l2d(q);
			if (sid == psco->sid)
				sum += psco->score;
		}

		(sv+i)->id  = sid;
		(sv+i)->sum = sum;
		i++;
		DEBUG(printf("%p[%u]: id = %u sum = %u\n",
		      sv, i-1, (sv+i-1)->id, (sv+i-1)->sum));
	}

	/* 1.4 sort */
	bubble_sort(sv, sv_sz);

	/*
	 * 2. init student view
	 */

	for (int i = 0; i < sv_sz; i++) {
		student_view_t *p =
		    (student_view_t *)malloc(sizeof (student_view_t));
		if (p == NULL) {
			fprintf(stderr, "failed to malloc:%s\n",
			        strerror(errno));
			rc = -1;
			goto done;
		}

		memset(p, 0, sizeof (student_view_t));
		p->sid = sv[i].id;

		DEBUG(printf("%d\t\t%p <--MALLOC (student_view)\n",
		      p->sid, p));

		list_t *lcs = NULL;
		for (list_t *lp = lsco; lp != NULL; lp = lp->next) {
			score_t *psco = list_l2d(lp);

			if (p->sid != psco->sid)
				continue;

			cs_t *q = (cs_t *)malloc(sizeof (cs_t));
			if (q == NULL) {
				fprintf(stderr, "failed to malloc:%s\n",
				        strerror(errno));
				rc = -1;
				goto done;
			}
			memset(q, 0, sizeof (cs_t));
			q->cid   = psco->cid;
			q->score = psco->score;

			DEBUG(printf("\t%d:%d\t%p <--malloc (student_view)\n",
			      q->cid, q->score, q));

			list_insert_tail(&lcs, q, offsetof(cs_t, link));
		}

		p->lcs = list_l2d(lcs);

		list_insert_tail(lview, p, offsetof(student_view_t, link));
	}

done:
	DEBUG(printf("%p <-- FREE@student_sort_view\n", sv));
	free(sv);

	return (rc);
}

int
init_course_view(list_t *lcou, list_t *lsco, list_t **lview)
{
	int rc = 0;

	/*
	 * 1. init course sort view array
	 */

	/* 1.1 get length of course list */
	uint16_t sv_sz = 0;
	for (list_t *p = lcou; p != NULL; p = p->next)
		sv_sz++;

	/* 1.2 malloc a new array */
	sort_view_t *sv = (sort_view_t *)malloc(sizeof (sort_view_t) * sv_sz);
	if (sv == NULL) {
		fprintf(stderr, "failed to malloc:%s\n", strerror(errno));
		return (-1);
	}
	memset(sv, 0, sizeof (sort_view_t) * sv_sz);
	DEBUG(printf("%p <-- MALLOC@course_sort_view\n", sv));

	/* 1.3 init the array */
	int i = 0;
	for (list_t *p = lcou; p != NULL; p = p->next) {
		course_t *pcou = list_l2d(p);

		uint16_t cid = pcou->cid;
		uint32_t sum = 0;

		for (list_t *q = lsco; q != NULL; q = q->next) {
			score_t *psco = list_l2d(q);
			if (cid == psco->cid)
				sum += psco->score;
		}

		(sv+i)->id = cid;
		(sv+i)->sum = sum;
		i++;
		DEBUG(printf("%p[%u]: id = %u sum = %u\n",
		      sv, i-1, (sv+i-1)->id, (sv+i-1)->sum));
	}

	/* 1.4 sort */
	bubble_sort(sv, sv_sz);

	/*
	 * 2. init course view
	 */

	for (int i = 0; i < sv_sz; i++) {
		course_view_t *p =
		    (course_view_t *)malloc(sizeof (course_view_t));
		if (p == NULL) {
			fprintf(stderr, "failed to malloc:%s\n",
			        strerror(errno));
			rc = -1;
			goto done;
		}
		memset(p, 0, sizeof (course_view_t));
		p->cid = sv[i].id;

		DEBUG(printf("%d\t\t%p <--MALLOC (course_view)\n",
		      p->cid, p));

		list_t *lss = NULL;
		for (list_t *lp = lsco; lp != NULL; lp = lp->next) {
			score_t *psco = list_l2d(lp);

			if (p->cid != psco->cid)
				continue;

			ss_t *q = (ss_t *)malloc(sizeof (ss_t));
			if (q == NULL) {
				fprintf(stderr, "failed to malloc:%s\n",
				        strerror(errno));
				rc = -1;
				goto done;
			}
			memset(q, 0, sizeof (ss_t));
			q->sid   = psco->sid;
			q->score = psco->score;

			DEBUG(printf("\t%d:%d\t%p <--malloc (course_view)\n",
			      q->sid, q->score, q));

			list_insert_tail(&lss, q, offsetof(ss_t, link));
		}

		p->lss = list_l2d(lss);

		list_insert_tail(lview, p, offsetof(course_view_t, link));
	}

done:
	DEBUG(printf("%p <-- FREE@course_sort_view\n", sv));
	free(sv);

	return (rc);
}

void
fini_course_view(list_t *lview)
{
	for (list_t *p = lview; p != NULL; p = p->next) {
		course_view_t *coup = list_l2d(p);

		DEBUG(printf("%d\t\t%p <--FREE   (course_view)\n",
		      coup->cid, coup));

		list_t *l = list_d2l(coup->lss, offsetof(ss_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			ss_t *ssp = list_l2d(q);

			DEBUG(printf("\t%d:%d\t%p <--free   (course_view)\n",
			      ssp->sid, ssp->score, ssp));
			free(ssp);
		}

		free(coup);
	}
}

void
fini_student_view(list_t *lview)
{
	for (list_t *p = lview; p != NULL; p = p->next) {
		student_view_t *stup = list_l2d(p);

		DEBUG(printf("%d\t\t%p <--FREE   (student_view)\n",
		      stup->sid, stup));

		list_t *l = list_d2l(stup->lcs, offsetof(cs_t, link));
		for (list_t *q = l; q != NULL; q = q->next) {
			cs_t *csp = list_l2d(q);

			DEBUG(printf("\t%d:%d\t%p <--free   (student_view)\n",
			      csp->cid, csp->score, csp));
			free(csp);
		}

		free(stup);
	}
}

char *
get_student_name(uint32_t id, list_t *l)
{
	for (list_t *p = l; p != NULL; p = p->next) {
		student_t *q = list_l2d(p);
		if (q->sid == id)
			return q->name;
	}

	return NULL;
}

char *
get_course_name(uint32_t id, list_t *l)
{
	for (list_t *p = l; p != NULL; p = p->next) {
		course_t *q = list_l2d(p);
		if (q->cid == id)
			return q->name;
	}

	return NULL;
}
