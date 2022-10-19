// explore.h
//
// These are area identity for explore points. Each area is allocated one
// chunk of id-space, which allows 10 individual points. If more than 10
// points are required, expand it with a new definition of new chunk in
// the following format: ID_XXX_n, where XXX is the area directory and n
// is the number of extension (2, 3, 4 ...)

#define ID_SNOW				0
#define ID_SNOWKEEP			10
#define ID_OLDPINE			20
#define ID_GRAVEYARD		30
#define ID_NEWCAMP			40
#define ID_WUTAN			50
#define ID_CHOYIN			60
#define ID_GOAT				70
#define ID_TAIYIE			80
#define ID_CLOUDY			90

#define __MAX_EXPLORE_ID__	100

#define EXPLORE(area,index)	(area + (index%10))
