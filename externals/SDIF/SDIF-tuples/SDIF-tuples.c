/*
Copyright (c) 1999, 2004.  The Regents of the University of California
(Regents). All Rights Reserved.

Permission to use, copy, modify, and distribute this software and its
documentation for educational, research, and not-for-profit purposes,
without fee and without a signed licensing agreement, is hereby granted,
provided that the above copyright notice, this paragraph and the
following two paragraphs appear in all copies, modifications, and distributions.
Contact The Office of Technology Licensing, UC Berkeley, 2150 Shattuck
Avenue, Suite 510, Berkeley, CA 94720-1620, (510) 643-7201, for commercial
licensing opportunities.

Written by Matt Wright, The Center for New Music and Audio Technologies,
University of California, Berkeley. Interpolation support by Ben "Jacobs".

     IN NO EVENT SHALL REGENTS BE LIABLE TO ANY PARTY FOR DIRECT, INDIRECT,
     SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES, INCLUDING LOST
     PROFITS, ARISING OUT OF THE USE OF THIS SOFTWARE AND ITS
     DOCUMENTATION, EVEN IF REGENTS HAS BEEN ADVISED OF THE POSSIBILITY OF
     SUCH DAMAGE.

     REGENTS SPECIFICALLY DISCLAIMS ANY WARRANTIES, INCLUDING, BUT NOT
     LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
     FOR A PARTICULAR PURPOSE. THE SOFTWARE AND ACCOMPANYING
     DOCUMENTATION, IF ANY, PROVIDED HEREUNDER IS PROVIDED "AS IS".
     REGENTS HAS NO OBLIGATION TO PROVIDE MAINTENANCE, SUPPORT, UPDATES,
     ENHANCEMENTS, OR MODIFICATIONS.

*/

/*

 3/26/99 SDIF-tuples.c -- the SDIF-tuples object
 A Max SDIF-buffer selector object
 
 version 0.2: includes reltime
 version 0.3: max_rows, only one complaint if empty
 version 0.3.1: "tuples time" now accepts int or float
 version 0.3.2: Does the right thing if no SDIF buffers exist at all
 version 0.4: uses -(DBL_MAX) instead of -9999999999999.9
 version 0.4b: compiled with CW 7.0
 version 0.5.0: added interpolation support (bj, 2004/04/01)
 version 0.5.1: cleanup (bj, 2004/06/22)
 
 Todo:
	more interpolation features
	
	More logical error reporting in Max window
 	
 -- */

#define SDIF_TUPLES_VERSION "0.5.1"
#define FINDER_NAME "SDIF-tuples"

#define MAX_NUM_COLUMNS 100
#define BIGGEST_OUTPUT_LIST 3000


#include <string.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include "ext.h"

/* Undo ext.h's macro versions of some of stdio.h: */
#undef fopen
#undef fclose
#undef fprintf
#undef fscanf
#undef fseek
#undef sprintf
#undef sscanf

#include "SDIF-buffer.h"  //  includes sdif.h, sdif-mem.h, sdif-buf.h
#include "sdif-util.h"
#include "sdif-interp.h"
#include "sdif-interp-implem.h"

#define VERY_SMALL ((sdif_float64) -(DBL_MAX))

/* #include <assert.h> */

#define assert(x) if (!(x)) { ouchstring("Assertion failed: %s, file %s, line %i\n", \
							             #x, __FILE__, __LINE__); } else {}

/* private types */

typedef enum {
  INTERP_MODE_NONE,
  INTERP_MODE_LINEAR,
  INTERP_MODE_LAGRANGE2,
  INTERP_MODE_LAGRANGE3,
  INTERP_MODE_END
} InterpMode;
  

/* My class definiton */

typedef struct _SDIFtuples {
	struct object t_ob;
	t_symbol *t_bufferSym;
	SDIFBuffer *t_buffer;
	SDIFbuf_Buffer t_buf;           //  provides API to manipulate buffer contents
	SDIFinterp_Interpolator t_it;   //  interpolator matrix
	Boolean t_itValid;              //  is interpolator matrix still valid?
 	void *t_out;
 	int t_errorreporting;
 	Boolean t_complainedAboutEmptyBufferAlready;
 	
 	/* State for what I'm supposed to output */
 	int t_concatenate;		/* One big list or one list per row? */
 	sdif_float64 t_time;	/* Which frame? */
 	Boolean t_reltime;			/* if nonzero, t_time is from 0 to 1 and should be scaled by length */
 	int t_direction;		/* Frame in which direction from t_time? */
 	int t_num_columns;		/* How many cols to output?  If 0, all of them */
 	int t_columns[MAX_NUM_COLUMNS];  /* Which ones? (1-based) */
 	InterpMode t_interp;       /* Interpolation mode */
 	Boolean t_mainMatrix;	  /* If true, output the main matrix in the frame */
 	char t_matrixType[4];   /* Type of matrix to output */
 	int t_max_rows;			/* Max # of rows to output */
} SDIFtuples;


static Symbol *ps_SDIFbuffer, *ps_SDIF_buffer_lookup, *ps_emptysymbol, *ps_concatenate,
	*ps_time, *ps_reltime, *ps_direction, *ps_columns, *ps_interp, *ps_max_rows;


/* global that holds the class definition */
static void *SDIFtuples_class;

/* prototypes for my functions */
void *SDIFtuples_new(Symbol *s, short argc, Atom *argv);
void SDIFtuples_free(SDIFtuples *x);
void *my_getbytes(int numBytes);
void my_freebytes(void *bytes, int size);
static void LookupMyBuffer(SDIFtuples *x);
static void SDIFtuples_set(SDIFtuples *x, Symbol *bufName);
static void SDIFtuples_errorreporting(SDIFtuples *x, long yesno);
static void SDIFtuples_concatenate(SDIFtuples *x, long yesno);
static void SDIFtuples_time(SDIFtuples *x, double t);
static void SDIFtuples_reltime(SDIFtuples *x, double t);
static void SDIFtuples_direction(SDIFtuples *x, long d);
static void SDIFtuples_columns(SDIFtuples *x, Symbol *s, short argc, Atom *argv);
static void SDIFtuples_matrix(SDIFtuples *x, Symbol *matrixType);
static void SDIFtuples_max_rows(SDIFtuples *x, long n);
static void SDIFtuples_tuples(SDIFtuples *x, Symbol *s, short argc, Atom *argv);
static SDIFmem_Matrix GetMatrix(SDIFtuples *x,
                                sdif_float64 time,
                                InterpMode mode,
                                int direction,
                                int *columns,
                                int num_columns);
static SDIFmem_Matrix GetMatrixWithoutInterpolation(SDIFtuples *x,
                                                    const char *desiredType,
                                                    sdif_float64 time,
                                                    int direction);
static SDIFmem_Matrix GetMatrixWithInterpolation(SDIFtuples *x,
                                                 const char *desiredType,
                                                 sdif_float64 time,
                                                 int *columns,
                                                 int num_columns,
                                                 InterpMode mode);
static void SetupInterpolator(SDIFtuples *x,
                              int srcColumns,
                              int *dstColumns, 
                              int num_dstColumns,
                              InterpMode mode);
void SDIFtuples_print(SDIFtuples *x);
void SDIFtuples_version(SDIFtuples *x);
void PrintOneFrame(SDIFmem_Frame f);
void PrintFrameHeader(SDIF_FrameHeader *fh);
void PrintMatrixHeader(SDIF_MatrixHeader *mh);


void main(fptr *fp)
{
  SDIFresult r;
  
	SDIFtuples_version(0);
	
	/* tell Max about my class. The cast to short is important for 68K */
	setup((t_messlist **)&SDIFtuples_class, (method)SDIFtuples_new, (method)SDIFtuples_free,
			(short)sizeof(SDIFtuples), 0L, A_GIMME, 0);
	
	/* bind my methods to symbols */
	addmess((method)SDIFtuples_set, "set", A_SYM, 0);	
	addmess((method)SDIFtuples_errorreporting, "errorreporting", A_LONG, 0);
	addmess((method)SDIFtuples_version, "version", 0);
	addmess((method)SDIFtuples_print, "print", 0);
	addmess((method)SDIFtuples_concatenate, "concatenate", A_LONG, 0);
	addmess((method)SDIFtuples_time, "time", A_FLOAT, 0);
	addmess((method)SDIFtuples_reltime, "reltime", A_FLOAT, 0);
	addmess((method)SDIFtuples_direction, "direction", A_LONG, 0);
	addmess((method)SDIFtuples_columns, "columns", A_GIMME, 0);
	addmess((method)SDIFtuples_max_rows, "max_rows", A_LONG, 0);	
	addmess((method)SDIFtuples_tuples, "tuples", A_GIMME, 0);
	addmess((method)SDIFtuples_matrix, "matrix", A_DEFSYM, 0);

  //  initialize SDIF libraries
	if (r = SDIF_Init()) {
		ouchstring("%s: Couldn't initialize SDIF library! %s", 
		           FINDER_NAME,
		           SDIF_GetErrorString(r));
    return;
	}
	
	if (r = SDIFmem_Init(my_getbytes, my_freebytes)) {
		post("� %s: Couldn't initialize SDIF memory utilities! %s", 
		     FINDER_NAME,
		     SDIF_GetErrorString(r));
    return;
	}
		
	if (r = SDIFbuf_Init()) {
		post("� %s: Couldn't initialize SDIF buffer utilities! %s", 
		     FINDER_NAME,
		     SDIF_GetErrorString(r));
		return;
	}
	
	if (r = SDIFinterp_Init()) {
		post("� %s: Couldn't initialize SDIF interpolation utilities! %s", 
		     FINDER_NAME,
		     SDIF_GetErrorString(r));
		return;
	}

	/* list object in the new object list */
	finder_addclass("Data", FINDER_NAME);
	
	ps_SDIFbuffer = gensym("SDIF-buffer");
	ps_SDIF_buffer_lookup = gensym("##SDIF-buffer-lookup");
	ps_emptysymbol  = gensym("");
	ps_concatenate = gensym("concatenate");
	ps_time = gensym("time");
	ps_reltime = gensym("reltime");
	ps_direction = gensym("direction");
	ps_columns = gensym("columns");
	ps_interp = gensym("interp");
	ps_max_rows = gensym("max_rows");
}

void *SDIFtuples_new(Symbol *dummy, short argc, Atom *argv) {
	SDIFtuples *x;
	int i;
	
	// post("SDIFtuples_new: %s, %ld args", s->s_name, (long) argc);
	
	x = newobject(SDIFtuples_class);
	x->t_errorreporting = 0;
	x->t_complainedAboutEmptyBufferAlready = FALSE;
	x->t_buffer = 0;
	x->t_out = listout(x);
	
	if (argc >= 1) {
		// First argument is name of SDIF-buffer
		if (argv[0].a_type != A_SYM) {
			post("� SDIF-tuples: argument must be name of an SDIF-buffer");
		} else {
			// post("* You want SDIF-buffer %s", argv[0].a_w.w_sym->s_name);
			x->t_bufferSym = argv[0].a_w.w_sym;
		}
	}
	
	x->t_buf = NULL;
	x->t_it = NULL;
	x->t_itValid = FALSE;

	x->t_mainMatrix = TRUE;
	x->t_concatenate = TRUE;
	x->t_time = VERY_SMALL;
	x->t_reltime = FALSE;
	x->t_direction = 1;
	x->t_interp = INTERP_MODE_NONE;
	x->t_max_rows = 1000000;
	
	x->t_num_columns = 0;
	for (i = 0; i < MAX_NUM_COLUMNS; ++i) {
		x->t_columns[i] = i;
	}

	return (x);
}

void SDIFtuples_free(SDIFtuples *x)
{
  if(x->t_it)
    SDIFinterp_Free(x->t_it);
}

static void *my_getbytes(int numBytes) {
	if (numBytes > SHRT_MAX) {
			return 0;
	}
	return (void *) getbytes((short) numBytes);
}

static void my_freebytes(void *bytes, int size) {
	freebytes(bytes, (short) size);
}

void SDIFtuples_version(SDIFtuples *x) {
	post("SDIF-tuples version " SDIF_TUPLES_VERSION " by Matt Wright");
	post("Copyright � 1999-2004 Regents of the University of California.");
}

static void LookupMyBuffer(SDIFtuples *x) {
#ifdef THE_S_THING_KLUDGE
	void *thing;
	
	thing = x->t_bufferSym->s_thing;
	
	if (thing) {
		if (ob_sym(thing) = ps_SDIFbuffer) {
			x->t_buffer = thing;
			return;
		}
	}
	x->t_buffer = 0;
#else
	SDIFBufferLookupFunction f;
	
	f = (SDIFBufferLookupFunction) ps_SDIF_buffer_lookup->s_thing;
	if (f == 0) {
		/* No SDIF buffer has ever been created yet. */
		x->t_buffer = 0;
	} else {
		x->t_buffer = (*f)(x->t_bufferSym);
	}
#endif

  //  get access to the SDIFbuf_Buffer instance
  if (x->t_buffer)
    x->t_buf = (x->t_buffer->BufferAccessor)(x->t_buffer);
  else
    x->t_buf = NULL;
}

static void SDIFtuples_set(SDIFtuples *x, Symbol *bufName) {
	x->t_buffer = 0;
	x->t_bufferSym = bufName;

	LookupMyBuffer(x);
	if (x->t_buffer == 0) {
		post("� SDIF-tuples: warning: there is no SDIF-buffer \"%s\"", bufName->s_name);
	}
	x->t_complainedAboutEmptyBufferAlready = FALSE;

}

static void SDIFtuples_errorreporting(SDIFtuples *x, long yesno) {
	x->t_errorreporting = yesno;
}

static void SDIFtuples_concatenate(SDIFtuples *x, long yesno) {
	x->t_concatenate = yesno;
}

static void SDIFtuples_time(SDIFtuples *x, double t) {
	x->t_time = (sdif_float64) t;
	x->t_reltime = FALSE;
}

static void SDIFtuples_reltime(SDIFtuples *x, double t) {
	x->t_time = (sdif_float64) t;
	x->t_reltime = TRUE;
}

static void SDIFtuples_direction(SDIFtuples *x, long d) {
	x->t_direction = d;
}

static void SDIFtuples_columns(SDIFtuples *x, Symbol *dummy, short argc, Atom *argv) {
	int i;

	/* First make sure all arguments are OK */
	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_LONG) {
			post("� SDIF-tuples: all arguments to columns must be ints!");
			return;
		}
		if (argv[i].a_w.w_long <= 0) {
			post("� SDIF-tuples: column number %d is no good.", argv[i].a_w.w_long);
			return;
		}
	}

	x->t_num_columns = argc;
	for (i = 0; i < argc; ++i) {
		x->t_columns[i] = argv[i].a_w.w_long;
	}
	

}

static void SDIFtuples_matrix(SDIFtuples *x, Symbol *matrixType) {
	if (matrixType == ps_emptysymbol) {
		x->t_mainMatrix = TRUE;
	} else {		
		/* Here's where I could implement user matrix names that aren't just the 4 bytes. */
		if (strlen(matrixType->s_name) != 4) {
			post("� SDIF-tuples: matrix %s is not 4 letters!", matrixType->s_name);
			return;
		}
		x->t_mainMatrix = FALSE;
		SDIF_Copy4Bytes(x->t_matrixType, matrixType->s_name);
	}
}

static void SDIFtuples_max_rows(SDIFtuples *x, long n) {
	x->t_max_rows = n;
}


static void SDIFtuples_tuples(SDIFtuples *x, Symbol *dummy, short argc, Atom *argv) {
	Boolean concatenate;
	sdif_float64 time;
	Boolean reltime;
	int direction;
	InterpMode interp;
	int num_columns;
	int columns[MAX_NUM_COLUMNS];
	int max_rows;
	int i, j;
	Atom outputArgs[BIGGEST_OUTPUT_LIST];
	short numArgs;
	SDIFmem_Matrix m;
	char desiredType[4];
	
	
	/* First, store my remembered parameters locally */
	concatenate = x->t_concatenate;
	time = x->t_time;
	reltime = x->t_reltime;
	direction = x->t_direction;
	interp = x->t_interp;
	num_columns = x->t_num_columns;
	for (i = 0; i < num_columns; ++i) {
 		columns[i] = x->t_columns[i];
 	}
 	max_rows = x->t_max_rows;
 	
	/* Now parse my arguments to replace these values */

	for (i = 0; i < argc; ++i) {
		if (argv[i].a_type != A_SYM) {
			post("� SDIF-tuples: bad arguments");
			return;
		}
		
		if (argv[i].a_w.w_sym == ps_concatenate) {
			if (i+1 < argc && argv[i+1].a_type == A_LONG) {
				concatenate = argv[i+1].a_w.w_long;
				++i;
			} else {
				post("� SDIF-tuples: bad argument after concatenate");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_interp) {
			if (i+1 < argc && argv[i+1].a_type == A_LONG) {
			  int arg = argv[i+1].a_w.w_long;
			  if(arg >= 0 && arg < INTERP_MODE_END)
			    interp = arg;
			  else {
  				post("� SDIF-tuples: unknown interp mode specified (%d)", arg);
  				return;
			  }
				++i;
			} else {
				post("� SDIF-tuples: bad argument after interp");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_time) {
			if (i+1 >= argc) {
				post("� SDIF-tuples: need an argument after time");
				return;
			} else if (argv[i+1].a_type == A_FLOAT) {
				time = (sdif_float64) argv[i+1].a_w.w_float;
				reltime = FALSE;
				++i;
			} else if (argv[i+1].a_type == A_LONG) {
				time = (sdif_float64) argv[i+1].a_w.w_long;
				reltime = FALSE;
				++i;
			} else {
				post("� SDIF-tuples: bad argument after time");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_reltime) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_FLOAT)) {
				time = (sdif_float64) argv[i+1].a_w.w_float;
				reltime = TRUE;
				++i;
			} else {
				post("� SDIF-tuples: bad argument after reltime");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_direction) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_LONG)) {
				direction = argv[i+1].a_w.w_long;
				++i;
			} else {
				post("� SDIF-tuples: bad argument after direction");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_max_rows) {
			if ((i+1 < argc) && (argv[i+1].a_type == A_LONG)) {
				max_rows = argv[i+1].a_w.w_long;
				++i;
			} else {
				post("� SDIF-tuples: bad argument after max_rows");
				return;
			}
		} else if (argv[i].a_w.w_sym == ps_columns) {
			++i;
			for (j = 0; i < argc && argv[i].a_type == A_LONG; ++j, ++i) {
				columns[j] = argv[i].a_w.w_long;
			}
			--i; // What we finally found wasn't a column number
			num_columns = j;
			if (num_columns == 0) {
				post("� SDIF-tuples: need at least one column!");
				return;
			}	
		} else {
			post("� SDIF-tuples: unrecognized argument %s", argv[i].a_w.w_sym->s_name);
			return;
		}
	}
	

	LookupMyBuffer(x);
	
	if (x->t_buffer == 0) {
		post("� SDIF-tuples: no buffer!");
		return;
	}
	
	if (reltime) {
	  sdif_float64 tMax;
	  
	  SDIFbuf_GetMaxTime(x->t_buf, &tMax);
		time *= tMax;
	}

  //  get the matrix (perform interpolation if requested)
  if(!(m = GetMatrix(x, time, interp, direction, columns, num_columns)))
    return;
	
	numArgs = 0;
	for (i = 0; (i < m->header.rowCount) && (i < max_rows); i++) {
		if (num_columns == 0) {
			/* Output all the columns */
			if (numArgs + m->header.columnCount >= BIGGEST_OUTPUT_LIST) {
				post("� SDIF-tuples: output list too big.");
				break;
			}
			for (j = 0; j < m->header.columnCount; ++j) {
				SETFLOAT(&(outputArgs[numArgs]), SDIFutil_GetMatrixCell(m, j, i));
				numArgs++;
			}
		} else {
			/* Output the desired columns */
			if (numArgs + num_columns >= BIGGEST_OUTPUT_LIST) {
				post("� SDIF-tuples: output list too big.");
				break;
			}
			for (j = 0; j<num_columns; ++j) {
				/* User's column numbers are 1-based, so sub 1 for array lookup: */
				SETFLOAT(&(outputArgs[numArgs]), SDIFutil_GetMatrixCell(m, columns[j] - 1, i));
				numArgs++;
			}
		}
		if (!concatenate) {
			outlet_list(x->t_out, 0L, numArgs, outputArgs);
			numArgs = 0;
		}
	}
	
	if (numArgs > 0) {
		outlet_list(x->t_out, 0L, numArgs, outputArgs);
	}
	
	//  release matrix memory
	SDIFmem_FreeMatrix(m);
}


static SDIFmem_Matrix GetMatrix(SDIFtuples *x,
                                sdif_float64 time,
                                InterpMode mode,
                                int direction,
                                int *columns,
                                int num_columns)
{
  SDIFmem_Matrix matrixOut;
  char desiredType[4];
  int i, j;

	LookupMyBuffer(x);
	
	if (x->t_buffer == 0) {
		post("� SDIF-tuples: no buffer!");
		return NULL;
	}
	
	//  what matrixType do we want?
	if (x->t_mainMatrix) {
	  SDIFmem_Frame f;
	  
		// The "main" matrix is the one whose type is the same as the frame type
		if(f = SDIFbuf_GetFirstFrame(x->t_buf))
  		SDIF_Copy4Bytes(desiredType, f->header.frameType);
  	else
  	  //  no frames in buffer, fail
  	  return NULL;
	} else {
		SDIF_Copy4Bytes(desiredType, x->t_matrixType);
	}	
	
	// post("* Looking for matrix type %c%c%c%c", desiredType[0], desiredType[1], desiredType[2], desiredType[3]);

  if(mode == INTERP_MODE_NONE)
  {
    //  try to get a matrix without interpolation
    if(!(matrixOut = GetMatrixWithoutInterpolation(x,
                                                   desiredType,
                                                   time,
                                                   direction)))
      return NULL;
  }
  else
  {
    //  try to compute an interpolated matrix
    if(!(matrixOut = GetMatrixWithInterpolation(x,
                                                desiredType,
                                                time,
                                                columns,
                                                num_columns,
                                                mode)))
      return NULL;
  }

	//  make sure requested columns actually exist in the matrix we found
	if (num_columns != 0) {
		for (j = 0; j < num_columns; ++j) {
			if (columns[j] > matrixOut->header.columnCount || columns[j] <= 0) {
				post("� SDIF-tuples: can't output column %d of a %d-column matrix!",
					 columns[j], matrixOut->header.columnCount);
				return NULL;
			}
		}
	}
	
  //  return result
  //  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()
  return matrixOut;
}


static SDIFmem_Matrix GetMatrixWithoutInterpolation(SDIFtuples *x,
                                                    const char *desiredType,
                                                    sdif_float64 time,
                                                    int direction)
{
  SDIFmem_Frame f;
  SDIFmem_Matrix m, matrixOut;
  
	//  get the frame
	if(!(f = (*(x->t_buffer->FrameLookup))(x->t_buffer, time, direction))) {
		if ((*(x->t_buffer->FrameLookup))(x->t_buffer, (sdif_float64) VERY_SMALL, 1) == 0) {
			if (!x->t_complainedAboutEmptyBufferAlready) {
				post("� SDIF-tuples: SDIF-buffer %s is empty", x->t_bufferSym->s_name);
				x->t_complainedAboutEmptyBufferAlready = TRUE;
			}
		} else {
			if (x->t_errorreporting) {
				post("� SDIF-tuples: SDIF-buffer %s has no frame at time %f", 
					x->t_bufferSym->s_name, (float) time);
			}
		}
		return NULL;
	}

	//  find the matrix
	for (m = f->matrices; m!= 0; m = m->next) {
		if (SDIF_Char4Eq(m->header.matrixType, desiredType)) {
			break;
		}
	}
	
	//  couldn't find the matrix
	if (m == 0) {
		post("� SDIF-tuples: no matrix of type %c%c%c%c in frame at time %f of SDIF-buffer %s",
			 desiredType[0], desiredType[1], desiredType[2], desiredType[3],
			 (float) f->header.time, x->t_bufferSym->s_name);
		return NULL;
	}

  //  we don't support matrix data types other than float	
  if ((m->header.matrixDataType >> 8) != SDIF_FLOAT)
  {
		post("� SDIF-tuples: Unsupported matrix data type (%x) in %c%c%c%c matrix ",
			 m->header.matrixDataType, 
			 desiredType[0], desiredType[1], desiredType[2], desiredType[3]);
		post("  in frame at time %f of SDIF-buffer %s (currently only float data is supported)",
			 (float) f->header.time, x->t_bufferSym->s_name);
		return NULL;
	}		
	  
	//  copy result to output matrix
	if(ESDIF_SUCCESS != SDIFutil_CloneMatrix(m, &matrixOut))
	  return NULL;
  
  //  return result
  //  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()
  return matrixOut;
}


static SDIFmem_Matrix GetMatrixWithInterpolation(SDIFtuples *x,
                                                 const char *desiredType,
                                                 sdif_float64 time,
                                                 int *columns,
                                                 int num_columns,
                                                 InterpMode mode)
{
  SDIFresult r;
  SDIFmem_Frame f;
  SDIFmem_Matrix m, matrixOut;
  sdif_float64 t1;
  sdif_int32 interpParam;
  
  //  find nearby frame (used as starting point for neighbor value searches)
  if(!(f = SDIFbuf_GetFrame(x->t_buf, time, ESDIF_SEARCH_BACKWARDS)))
    return NULL;
  
  //  try to find a matrix of desired type in this frame or earlier
  if(!(m = SDIFbuf_GetMatrixNearby(f, desiredType, time, ESDIF_SEARCH_BACKWARDS, &t1)))
    return NULL;

  //  we don't support matrix data types other than float	
  if ((m->header.matrixDataType >> 8) != SDIF_FLOAT)
  {
		post("� SDIF-tuples: Unsupported matrix data type (%x) in %c%c%c%c matrix ",
			 m->header.matrixDataType, 
			 desiredType[0], desiredType[1], desiredType[2], desiredType[3]);
		post("  in frame at time %f of SDIF-buffer %s (currently only float data is supported)",
			 (float) f->header.time, x->t_bufferSym->s_name);
		return NULL;
	}		
	  
  //  build the interpolator matrix, if anything has changed since last time
  if(!(x->t_itValid))
  {
    SetupInterpolator(x, m->header.columnCount, columns, num_columns, mode);
    if(!(x->t_it))
      return NULL;
  }
  
  //  create the output matrix (find dimensions by looking at nearby matrix instance)
  if(!(matrixOut = SDIFutil_CreateMatrix(m->header.columnCount,
                                         m->header.rowCount,
                                         SDIF_FLOAT64,
                                         desiredType)))
    return NULL;
  
  //  compute the interpolated matrix
  switch(mode)
  {
    case INTERP_MODE_LINEAR:
      interpParam = 0;   //  ignored by interpolator anyway
      break;
    case INTERP_MODE_LAGRANGE2:
      interpParam = 2;   //  Lagrange/Waring degree == 2
      break;
    case INTERP_MODE_LAGRANGE3:
      interpParam = 3;   //  Lagrange/Waring degree == 3
      break;
    default:
      interpParam = 0;
  }
  if(ESDIF_SUCCESS != SDIFinterp_GetMatrixNearby(x->t_it,
                                                 f,
                                                 desiredType,
                                                 time,
                                                 ESDIF_NAN_ACTION_KEEP_LOOKING,
                                                 matrixOut,
                                                 interpParam))
  {
    SDIFmem_FreeMatrix(matrixOut);
    return NULL;
  }
  
  //  return result
  //  NOTE: caller is responsible for calling SDIFmem_FreeMatrix()
  return matrixOut;
}


static void SetupInterpolator(SDIFtuples *x,
                              int srcColumns,
                              int *dstColumns, 
                              int num_dstColumns,
                              InterpMode mode)
{
  //  release old interpolator, if any
  if(x->t_it)
    SDIFinterp_Free(x->t_it);
  
  //  create new interpolator
  if(!(x->t_it = SDIFinterp_Create(srcColumns)))
    return;

  /////
  /////  to avoid interpolating unused columns, should fill in unused columns with NULL
  /////
  
  switch(mode)
  {
    case INTERP_MODE_LINEAR:
      SDIFinterp_SetAllInterpolatorFn(x->t_it, LinearInterpolator);
      break;
    case INTERP_MODE_LAGRANGE2:
    case INTERP_MODE_LAGRANGE3:
      SDIFinterp_SetAllInterpolatorFn(x->t_it, LagrangeInterpolator);
      break;
    default:
      SDIFinterp_SetAllInterpolatorFn(x->t_it, NULL);
      break;
  }
  
  //  set this flag if we want to avoid creating a new interpolator on every matrix request:
  //  t_itValid = TRUE;
}


/* Printing stuff ***************************/

void SDIFtuples_print(SDIFtuples *x) {
	SDIFmem_Frame f;
	
	if (x->t_bufferSym == 0) {
		post("SDIFtuples: no SDIF buffer name specified");
	} else {
		LookupMyBuffer(x);
		if (x->t_buffer == 0) {
			post("� SDIFtuples: \"%s\" is not an SDIF buffer.", x->t_bufferSym->s_name);
		} else {
		  sdif_float64 tMin, tMax;
		  
			// post("* ob_sym(x->t_buffer)->s_name: %s", ob_sym(x->t_buffer)->s_name);
			
			post("SDIFtuples: SDIF-buffer \"%s\"", x->t_buffer->s_myname->s_name);
			if(f = SDIFbuf_GetFirstFrame(x->t_buf))
  			post("   Stream ID %ld, Frame Type %c%c%c%c", x->t_buffer->streamID,
  				f->header.frameType[0], f->header.frameType[1], f->header.frameType[2], f->header.frameType[3]);
			SDIFbuf_GetMinTime(x->t_buf, &tMin);
			SDIFbuf_GetMaxTime(x->t_buf, &tMax);
			post("   Min time %g, Max time %g", tMin, tMax);
			
			// post("Calling FrameLookup(VERY_SMALL, 1)");
			// f = (*(x->t_buffer->FrameLookup))(x->t_buffer, (sdif_float64) VERY_SMALL, 1);
			// post("It returned %p", f);
			
			for (/* f already set */; f != NULL; f = f->next) {
				PrintOneFrame(f);
			}
		}
	}
}

void PrintOneFrame(SDIFmem_Frame f) {
	SDIFmem_Matrix m;
	if (f == 0) {
		post("PrintOneFrame: null SDIFmem_Frame pointer");
		return;
	}
	// post("SDIF frame at %p, prev is %p, next is %p", f, f->prev, f->next);
	PrintFrameHeader(&(f->header));
	
	for (m = f->matrices; m != 0; m=m->next) {
		PrintMatrixHeader(&(m->header));
	}
}

void PrintFrameHeader(SDIF_FrameHeader *fh) {
	post(" Frame header: type %c%c%c%c, size %ld, time %g, stream ID %ld, %ld matrices",
		 fh->frameType[0], fh->frameType[1], fh->frameType[2], fh->frameType[3], 
		 fh->size, fh->time, fh->streamID, fh->matrixCount);
}
	
void PrintMatrixHeader(SDIF_MatrixHeader *mh) {
	post("  Matrix header: type %c%c%c%c, data type %ld, %ld rows, %ld cols",
		 mh->matrixType[0], mh->matrixType[1], mh->matrixType[2], mh->matrixType[3],
		 mh->matrixDataType, mh->rowCount, mh->columnCount);
}
