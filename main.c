#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#define ROWS 30
#define COLS 80
#define MAX 100

#define RECT 1
#define LINE 2
#define CIRC 3
#define TRI  4

typedef struct {
    int id, type, active;
    int x, y, w, h;
    int ex[4];
} Sh;

char cv[ROWS][COLS];
Sh sh[MAX];
int cnt=0, nid=1;

// fill canvas with dots
void initcv() {
    int i,j;
    for(i=0;i<ROWS;i++)
        for(j=0;j<COLS;j++)
            cv[i][j]='.';
}

// safe put pixel
void put(int r, int c, char ch) {
    if(r>=0&&r<ROWS&&c>=0&&c<COLS)
        cv[r][c]=ch;
}

void showcv() {
    int i,j;
    printf("\n");
    for(i=0;i<ROWS;i++) {
        for(j=0;j<COLS;j++)
            printf("%c",cv[i][j]);
        printf("\n");
    }
}

// bresenham line
void dline(int x1,int y1,int x2,int y2,char ch) {
    int dx=abs(x2-x1), dy=abs(y2-y1);
    int sx=(x1<x2)?1:-1, sy=(y1<y2)?1:-1;
    int err=dx-dy, e2;
    while(1) {
        put(y1,x1,ch);
        if(x1==x2&&y1==y2) break;
        e2=2*err;
        if(e2>-dy){err-=dy; x1+=sx;}
        if(e2<dx) {err+=dx; y1+=sy;}
    }
}

// midpoint circle
void dcirc(int cx,int cy,int r,char ch) {
    int x=0,y=r,d=1-r;
    while(x<=y) {
        put(cy+y,cx+x,ch); put(cy+y,cx-x,ch);
        put(cy-y,cx+x,ch); put(cy-y,cx-x,ch);
        put(cy+x,cx+y,ch); put(cy+x,cx-y,ch);
        put(cy-x,cx+y,ch); put(cy-x,cx-y,ch);
        if(d<0) d+=2*x+3;
        else {d+=2*(x-y)+5; y--;}
        x++;
    }
}

// rect border
void drect(int x,int y,int w,int h,char ch) {
    int i,j;
    for(j=x;j<x+w;j++) {
        put(y,j,ch);
        put(y+h-1,j,ch);
    }
    for(i=y;i<y+h;i++) {
        put(i,x,ch);
        put(i,x+w-1,ch);
    }
}

// triangle = 3 lines
void dtri(int x1,int y1,int x2,int y2,int x3,int y3,char ch) {
    dline(x1,y1,x2,y2,ch);
    dline(x2,y2,x3,y3,ch);
    dline(x3,y3,x1,y1,ch);
}

void drawsh(Sh *s) {
    if(!s->active) return;
    switch(s->type) {
        case RECT: drect(s->x,s->y,s->w,s->h,'*'); break;
        case LINE: dline(s->x,s->y,s->w,s->h,'#'); break;
        case CIRC: dcirc(s->x,s->y,s->w,'O'); break;
        case TRI:  dtri(s->x,s->y,s->w,s->h,s->ex[0],s->ex[1],'@'); break;
    }
}

void redraw() {
    int i;
    initcv();
    for(i=0;i<cnt;i++)
        drawsh(&sh[i]);
}

const char *tname(int t) {
    switch(t) {
        case RECT: return "Rectangle";
        case LINE: return "Line";
        case CIRC: return "Circle";
        case TRI:  return "Triangle";
        default:   return "?";
    }
}

// add shapes
void addrect() {
    Sh s;
    memset(&s,0,sizeof(s));
    s.id=nid++; s.type=RECT; s.active=1;
    printf("x y: "); scanf("%d%d",&s.x,&s.y);
    printf("w h: "); scanf("%d%d",&s.w,&s.h);
    sh[cnt++]=s;
    redraw();
    printf("rect added id=%d\n",s.id);
}

void addline() {
    Sh s;
    memset(&s,0,sizeof(s));
    s.id=nid++; s.type=LINE; s.active=1;
    printf("x1 y1: "); scanf("%d%d",&s.x,&s.y);
    printf("x2 y2: "); scanf("%d%d",&s.w,&s.h);
    sh[cnt++]=s;
    redraw();
    printf("line added id=%d\n",s.id);
}

void addcirc() {
    Sh s;
    memset(&s,0,sizeof(s));
    s.id=nid++; s.type=CIRC; s.active=1;
    printf("cx cy: "); scanf("%d%d",&s.x,&s.y);
    printf("r: ");     scanf("%d",&s.w);
    sh[cnt++]=s;
    redraw();
    printf("circle added id=%d\n",s.id);
}

void addtri() {
    Sh s;
    memset(&s,0,sizeof(s));
    s.id=nid++; s.type=TRI; s.active=1;
    printf("p1 x y: "); scanf("%d%d",&s.x,&s.y);
    printf("p2 x y: "); scanf("%d%d",&s.w,&s.h);
    printf("p3 x y: "); scanf("%d%d",&s.ex[0],&s.ex[1]);
    sh[cnt++]=s;
    redraw();
    printf("tri added id=%d\n",s.id);
}

void delsh() {
    int id,i;
    printf("id to delete: "); scanf("%d",&id);
    for(i=0;i<cnt;i++) {
        if(sh[i].id==id&&sh[i].active) {
            sh[i].active=0;
            redraw();
            printf("deleted id=%d\n",id);
            return;
        }
    }
    printf("not found\n");
}

void modsh() {
    int id,i;
    printf("id to modify: "); scanf("%d",&id);
    for(i=0;i<cnt;i++) {
        if(sh[i].id!=id||!sh[i].active) continue;
        Sh *s=&sh[i];
        printf("modifying %s id=%d\n",tname(s->type),id);
        switch(s->type) {
            case RECT:
                printf("new x y: "); scanf("%d%d",&s->x,&s->y);
                printf("new w h: "); scanf("%d%d",&s->w,&s->h);
                break;
            case LINE:
                printf("new x1 y1: "); scanf("%d%d",&s->x,&s->y);
                printf("new x2 y2: "); scanf("%d%d",&s->w,&s->h);
                break;
            case CIRC:
                printf("new cx cy: "); scanf("%d%d",&s->x,&s->y);
                printf("new r: ");     scanf("%d",&s->w);
                break;
            case TRI:
                printf("new p1 x y: "); scanf("%d%d",&s->x,&s->y);
                printf("new p2 x y: "); scanf("%d%d",&s->w,&s->h);
                printf("new p3 x y: "); scanf("%d%d",&s->ex[0],&s->ex[1]);
                break;
        }
        redraw();
        printf("modified\n");
        return;
    }
    printf("not found\n");
}

void listsh() {
    int i,f=0;
    printf("\nid   type        info\n");
    for(i=0;i<cnt;i++) {
        Sh *s=&sh[i];
        if(!s->active) continue;
        f=1;
        printf("%-4d %-10s ",s->id,tname(s->type));
        switch(s->type) {
            case RECT: printf("x=%d y=%d w=%d h=%d",s->x,s->y,s->w,s->h); break;
            case LINE: printf("(%d,%d)->(%d,%d)",s->x,s->y,s->w,s->h); break;
            case CIRC: printf("cx=%d cy=%d r=%d",s->x,s->y,s->w); break;
            case TRI:  printf("(%d,%d)(%d,%d)(%d,%d)",s->x,s->y,s->w,s->h,s->ex[0],s->ex[1]); break;
        }
        printf("\n");
    }
    if(!f) printf("no shapes\n");
}

void addmenu() {
    int c;
    printf("\n1.rect 2.line 3.circle 4.tri\nchoice: ");
    scanf("%d",&c);
    switch(c) {
        case 1: addrect(); break;
        case 2: addline(); break;
        case 3: addcirc(); break;
        case 4: addtri();  break;
        default: printf("invalid\n");
    }
}

int main() {
    int c;
    initcv();
    printf("canvas - rect=* line=# circle=O tri=@\n");
    while(1) {
        printf("\n1.add 2.delete 3.modify 4.show 5.list 6.exit\nchoice: ");
        scanf("%d",&c);
        switch(c) {
            case 1: addmenu(); break;
            case 2: delsh();   break;
            case 3: modsh();   break;
            case 4: showcv();  break;
            case 5: listsh();  break;
            case 6: return 0;
            default: printf("invalid\n");
        }
    }
    return 0;
}