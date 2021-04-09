struct list
	{
	int x,y;
	float fx, fy;
	struct list *next, *last;
	float time;
	float velocity;
	float n;
	};

typedef struct list LIST;
