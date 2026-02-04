// vim: set syntax=lpc :

#ifndef	ATTRIBUTE_H
#define	ATTRIBUTE_H

/* 定義一個人物「屬性」值的個數 */
#define	NUM_ATTRIBUTES	8

/* 屬性值的最大及最小值 */
#define	ATTRVAL_MIN	1
#define ATTRVAL_MAX	50

varargs void init_attribute(mapping base);
int set_attr(string what, int value);
varargs int query_attr(string what, int raw);


#endif	/* ATTRIBUTE_H */
