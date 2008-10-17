/*---------------------------------------------------------------------------*
 |              PDFlib - A library for generating PDF on the fly             |
 +---------------------------------------------------------------------------+
 | Copyright (c) 1997-2006 Thomas Merz and PDFlib GmbH. All rights reserved. |
 +---------------------------------------------------------------------------+
 |                                                                           |
 |    This software is subject to the PDFlib license. It is NOT in the       |
 |    public domain. Extended versions and commercial licenses are           |
 |    available, please check http://www.pdflib.com.                         |
 |                                                                           |
 *---------------------------------------------------------------------------*/

/* $Id$
 *
 * PDFlib generic container classes
 *
 */

#ifndef PC_CONTAIN_H
#define PC_CONTAIN_H

/* container entry descriptor
*/
typedef struct
{
    size_t	size;

    void	(*reclaim)(void *item);
    void	(*release)(void *context, void *item);
    int		(*compare)(const void *lhs, const void *rhs);
} pdc_ced;


/* callback functions for the "for_each" methods
*/
typedef void	(*pdc_for_each_cb)(void *context, void *item);


/**************************** avl tree class ****************************/

typedef struct	pdc_avl_s pdc_avl;

pdc_avl *	pdc_avl_new(pdc_core *pdc, const pdc_ced *ced, void *context);
void		pdc_avl_delete(pdc_avl *t);
int		pdc_avl_size(const pdc_avl *t);
void *		pdc_avl_insert(pdc_avl *t, const void *item);
void		pdc_avl_for_each(const pdc_avl *t, pdc_for_each_cb cb);


/***************************** vector class *****************************/

typedef struct	pdc_vtr_s pdc_vtr;

typedef struct
{
    int		init_size;
    int		chunk_size;
    int		ctab_incr;
} pdc_vtr_parms;

void		pdc_vtr_dflt_parms(pdc_vtr_parms *vp);

pdc_vtr *	pdc_vtr_new(pdc_core *pdc, const pdc_ced *ced, void *context,
		    const pdc_vtr_parms *parms);

void		pdc_vtr_delete(pdc_vtr *v);
int		pdc_vtr_size(const pdc_vtr *v);
void		pdc_vtr_resize(pdc_vtr *v, int size);
void		pdc_vtr_pop(pdc_vtr *v);

/* don't use the pdc__vtr_xxx() functions directly.
** use the respective pdc_vtr_xxx() macros below.
*/
void *		pdc__vtr_at(const pdc_vtr *v, int idx);
void *		pdc__vtr_top(const pdc_vtr *v);
void *		pdc__vtr_push(pdc_vtr *v);


/* <type>	pdc_vtr_at(const pdc_vtr *v, int idx, <type>);
**
**		(<type>) v[idx]
*/
#define		pdc_vtr_at(v, idx, type)			\
		(*((type *) pdc__vtr_at(v, idx)))


/* <type>	pdc_vtr_top(const pdc_vtr *v, <type>);
**
**		(<type>) v[vsize-1]
*/
#define		pdc_vtr_top(v, type)				\
		(*((type *) pdc__vtr_top(v)))


/* void		pdc_vtr_push(pdc_vtr *v, item, <type>);
**
**		(<type>) v[vsize++] = item
*/
#define		pdc_vtr_push(v, item, type)			\
		(*((type *) pdc__vtr_push(v)) = item)


/* <type> *	pdc_vtr_incr(pdc_vtr *v, <type>);
**
**		(<type> *) &v[vsize++]
*/
#define		pdc_vtr_incr(v, type)				\
		((type *) pdc__vtr_push(v))

#endif	/* PC_CONTAIN_H */
