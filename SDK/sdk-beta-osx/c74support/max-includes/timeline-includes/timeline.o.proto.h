	/* timeline.o.c */
#ifndef NOTIMELINE
void timeline_activate(Timeline, short);
void timeline_chkmenu(Timeline, struct menuinfo *);
void timeline_clean(Timeline);
void timeline_click(Timeline, Point, short, short);
void timeline_clockFn(Timeline,long);
void timeline_close(Timeline);
void timeline_dblclick(Timeline);
void timeline_dirty(Timeline);
void *timeline_display(Timeline,long);
void timeline_displayNow(Timeline,long,short);
void timeline_divider(Timeline,long);
void timeline_down(Timeline);
void timeline_draw(Timeline);
void 	timeline_drawImmediate(Timeline,short,short);
void timeline_fixwidth(Timeline);
void timeline_filename(Timeline,char *,short,short);
void timeline_free(Timeline);
void *timeline_front(Timeline);
void timeline_idle(Timeline, Point, short);
void timeline_int(Timeline, long);
void timeline_invis(Timeline);
void timeline_key(Timeline,short,short,short);
void timeline_left(Timeline,long);
void timeline_locate(Timeline,long);
void timeline_loop(Timeline);
void timeline_maxTime(Timeline);
long timeline_modeControl(Timeline,short);
void *timeline_new(Symbol *, short, Atom *);
void timeline_mouseup(Timeline, Point, short);
void timeline_oksize(Timeline, short *, short *);
void timeline_otclick(Timeline, Point);
void timeline_out(Timeline, Symbol *, short, Atom *);
void timeline_qfn(Timeline x);
void timeline_play(Timeline);
void timeline_post(Timeline,char*);
void timeline_recordBuffer(Timeline x,long bufSize);
void timeline_record(Timeline x);
void timeline_right(Timeline,long);
void timeline_scale(Timeline,long);
void timeline_scroll(Timeline);
void timeline_solo(Timeline,long);
void timeline_stop(Timeline);
void timeline_totime(Timeline, long);
void timeline_time(Timeline, long *);
void timeline_timeOffset(Timeline,long);
void timeline_timeFormat(Timeline,long,long);
void *timeline_TLEnd(Timeline);
void *timeline_TREnd(Timeline);
void timeline_up(Timeline);
void timeline_update(Timeline);
void timeline_vis(Timeline);
void timeline_wsize(Timeline, short, short);
void timeline_zoom(Timeline,long);
void timeline_zoomIn(Timeline);
void timeline_zoomOut(Timeline);
void c_timeline(void);

#endif

