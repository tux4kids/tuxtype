extern int SNOW_on;
extern SDL_Rect SNOW_rects[2000];
extern int SNOW_add( SDL_Rect *rs, int num );
extern void SNOW_toggle( void );
extern void SNOW_draw( void );
extern void SNOW_erase( void );
extern void SNOW_setBkg( SDL_Surface *img );
extern void SNOW_init( void );
extern void SNOW_update( void );
