#ifndef MANAGER_HEADER_INCLUDED
#define MANAGER_HEADER_INCLUDED

#define MODE_AFF_IDLE     (0)
#define MODE_AFF_DISPLAY  (1)
#define MODE_AFF_REMOTE   (2)
#define MODE_AFF_AUTO     (3)

extern int mode_aff;

extern void manager_init(void);
extern void manager_run(void);

#endif
