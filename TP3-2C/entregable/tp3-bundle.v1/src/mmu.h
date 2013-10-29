/* ** por compatibilidad se omiten tildes **
================================================================================
 TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
================================================================================
  definicion de funciones del manejador de memoria
*/

#ifndef __MMU_H__
#define __MMU_H__

typedef struct str_pd_entry {
	unsigned char	p:1;
	unsigned char	rw:1;
	unsigned char	us:1;
	unsigned char	pwt:1;
	unsigned char	pcd:1;
	unsigned char	a:1;
	unsigned char	ignr:1;
	unsigned char	ps:1;
	unsigned char	g:1;
	unsigned char	ignr_3:3;
	unsigned int	base_12_31:20;
} __attribute__((__packed__, aligned (8))) pd_entry;

typedef struct str_pt_entry {
	unsigned char	p:1;
	unsigned char	rw:1;
	unsigned char	us:1;
	unsigned char	pwt:1;
	unsigned char	pcd:1;
	unsigned char	a:1;
	unsigned char	d:1;
	unsigned char	pat:1;
	unsigned char	g:1;
	unsigned char	ignr_3:3;
	unsigned int	base_12_31:20;
} __attribute__((__packed__, aligned (8))) pt_entry;


extern pd_entry* pd;
extern pt_entry* pt;
#endif	/* !__MMU_H__ */
