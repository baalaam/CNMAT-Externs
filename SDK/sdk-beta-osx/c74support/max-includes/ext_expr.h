/* ext_expr.h -- header file for writing expressions */

#define MAXMAX		/* <z> added to users of this stuff won't be screwed */

#define	MAX_VARS	9

struct ex_funcs {
	char *f_name;		/* function name */
	long (*f_func)();	/* the real function performing the function */
	long f_argc;		/* number of arguments */
};
#define	name_ok(c)	(((c)=='_') || ((c)>='a' && (c)<='z') || \
			((c)>='A' && (c)<='Z') || ((c) >= '0' && (c) <= '9'))
#define unary_op(x)	((x) == OP_NOT || (x) == OP_NEG || (x) == OP_UMINUS)

typedef struct ex_ex {
	union {
		long v_int;
		float v_flt;
		long op;
		char **ptr;
	} ex_cont;		/* content */
#define	ex_int		ex_cont.v_int
#define	ex_flt		ex_cont.v_flt
#define	ex_op		ex_cont.op
#define	ex_ptr		ex_cont.ptr
	long ex_type;		/* type of the node */
} Ex_ex;
#define	exNULL	((struct ex_ex *)0)

/* defines for ex_type */
#define	ET_INT		0x1		/* an int */
#define	ET_FLT		0x2		/* a float */
#define ET_OP		0x3		/* operator */
#define	ET_STR		0x4		/* string */
#define ET_TBL		0x5		/* a table, the content is a pointer */
#define	ET_FUNC		0x6		/* a function */
#define	ET_SYM		0x7		/* symbol ("string") */
#define	ET_VSYM		0x8		/* variable symbol ("$s?") */

typedef struct expr {
	struct object exp_ob;
	void *exp_outlet;
	struct ex_ex **exp_stack;
	struct ex_ex exp_var[MAX_VARS];
	struct ex_ex exp_res;		/* the result of last evaluation */
} Expr;
