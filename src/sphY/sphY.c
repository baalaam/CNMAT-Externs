/**
 
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 NAME: sphY
 DESCRIPTION: Evaluates the real-valued spherical harmonics up to order N at a point (theta, phi)
 AUTHORS: Franz Zotter, Andy Schmeder
 COPYRIGHT_YEARS: 2006-2008
 VERSION 0.1: Ported to MaxMSP
 VERSION 0.2: Improve help patch
 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 
 */
#define NAME "sphY"
#define DESCRIPTION "Evaluates the real-valued spherical harmonics up to order N at a point (theta, phi)"
#define AUTHORS "Franz Zotter, Andy Schmeder"
#define COPYRIGHT_YEARS "2006-08,12,13"

#include <stdlib.h>
#include <math.h>

// MaxMSP
#include "ext.h"
#include "ext_obex.h"


// Franz Zotter's SH library
#include "sh.h"

// CNMAT version control
#include "version.h"


t_symbol* ps_index;
t_symbol* ps_r;

typedef struct _sphY
{
    
    t_object o_ob;
    
    t_object* in_p[1];
    long in_i_unsafe;
    
    t_object* out_p[1];
    
    sh* dummy;
    sh* shp;
    
    t_atom *list_buf;
    t_atom *list_i_buf;
    int order;
    int len;
    
} sphY;

static t_class *sphY_class;

void* sphY_new(t_symbol* s, short argc, t_atom* argv);
void sphY_free(sphY* x);
void sphY_assist(sphY* x, void *box, long msg, long arg, char *dstString);
void sphY_list(sphY* x, t_symbol* mess, short argc, t_atom* argv);
void sphY_index(sphY* x, t_symbol* mess, short argc, t_atom* argv);

// setup
int main(void)
{
    
    // announce copyright
    version_post_copyright();
    
    // setup
    sphY_class = class_new("sphY", (method)sphY_new, (method)sphY_free, (short)sizeof(sphY), 0L, A_GIMME, 0);
    
    // reset
    class_addmethod(sphY_class, (method)sphY_list, "eval", A_GIMME, 0);
    
    // reset
    class_addmethod(sphY_class, (method)sphY_index, "index", A_GIMME, 0);
    
    // tooltip helper
    class_addmethod(sphY_class, (method)sphY_assist, "assist", A_CANT, 0);
    
    ps_index = gensym("index");
    ps_r = gensym("r");
    
    class_register(CLASS_BOX, sphY_class);
    return 0;
}

#define ASSIST_INLET 1
#define ASSIST_OUTLET 2

void sphY_assist(sphY *x, void *box, long msg, long arg, char *dstString) {
    
    if (msg == ASSIST_INLET) {
        if(arg == 0) {
            sprintf(dstString, "message 'eval theta phi': theta in [0, Pi] (declination), phi in [0, 2Pi] (azimuth), or message 'index'");
        }
    } else if (msg == ASSIST_OUTLET) {
        if(arg == 0) {
            sprintf(dstString, "list of real-valued spherical harmonics, with length (order+1)^2, or list of order, degree pairs");
        }
    } else {
        object_post((t_object *)x, "sphY_assist: unrecognized message %ld", msg);
    }
    
}

void *sphY_new(t_symbol* s, short argc, t_atom *argv)
{
    
    sphY *x;
    int i;
    
    x = (sphY*) object_alloc(sphY_class);
    if(!x){
	    return NULL;
    }
    x->order = 0;
    
    for(i = 0; i < argc; i++) {
        
        if(argv[i].a_type == A_SYM) {
            
            // @order
            if(strcmp(argv[i].a_w.w_sym->s_name, "@order") == 0) {
                
                if(i + 1 < argc) {
                    i++;
                    
                    if(argv[i].a_type == A_LONG) {
                        x->order = argv[i].a_w.w_long;
                    } else {
                        object_post((t_object *)x, "sphY: expected int for order");
                    }
                } else {
                    object_post((t_object *)x, "sphY: missing arg after order");
                }
            }
            
        }
        
    }
    
    // ensure positive (else crashy)
    x->order = (x->order < 0) ? 0 : x->order;
    
    // try to allocate
    if ((x->shp = newSH(x->order))==0) {
        return 0;
    }
    
    x->len = (x->order+1)*(x->order+1);
    x->list_buf = (t_atom*)calloc(x->len, sizeof(t_atom));
    x->list_i_buf = (t_atom*)calloc(2*x->len, sizeof(t_atom));
    
    x->out_p[0] = outlet_new(x, "list");
    
    x->dummy = x->shp;
    
    return (void *)x;
}

void sphY_free(sphY *x)
{
    if (x->shp != NULL) {
        deleteSH(x->shp);
    }
    if (x->list_buf != NULL) {
        free(x->list_buf);
    }
    if (x->list_i_buf != NULL) {
        free(x->list_i_buf);
    }
}

void sphY_list(sphY* x, t_symbol* mess, short argc, t_atom* argv)
{
    
    int n,m,mn;
    double phi,theta,value;
    if(argc == 2) {
        
        theta=(double)(argv[0].a_w.w_float);
        phi=(double)(argv[1].a_w.w_float);
        
        for(n=0, mn=0; n <= x->order; n++) {
            for (m = -n; m <= n; m++, mn++) {
                value=sHEvaluate(x->shp, n, m, phi, theta);
                atom_setfloat(&x->list_buf[mn], (float)value);
            }
        }
        outlet_anything(x->out_p[0], ps_r, x->len, x->list_buf);
    }
    else {
        object_post((t_object *)x, "sphY: invalid number of input arguments, expected two, got %d", argc);
    }
}

void sphY_index(sphY* x, t_symbol* mess, short argc, t_atom* argv)
{
    
    int n, m, mn;
    
    for(n=0, mn=0; n <= x->order; n++) {
        for (m = -n; m <= n; m++, mn++) {
            atom_setlong(&x->list_i_buf[2*mn], n);
            atom_setlong(&x->list_i_buf[2*mn+1], m);
        }
    }
    
    outlet_anything(x->out_p[0], ps_index, 2*x->len, x->list_i_buf);
    
}

