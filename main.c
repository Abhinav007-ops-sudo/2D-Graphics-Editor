#include <stdio.h>

#define ROWS 30
#define COLS 80
#define MAX_RECTS 100

typedef struct
{
    int id;
    int x;
    int y;
    int width;
    int height;
    int active;
} Rectangle;

char canvas[ROWS][COLS];
Rectangle rects[MAX_RECTS];

int rectCount = 0;
int nextId = 1;

void initializeCanvas()
{
    int i, j;
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas()
{
    int i, j;
    for(i = 0; i < ROWS; i++)
    {
        for(j = 0; j < COLS; j++)
        {
            printf("%c", canvas[i][j]);
        }
        printf("\n");
    }
}

void drawRectangle(Rectangle r)
{
    int i, j;
    if(!r.active)
        return;
    for(j = r.x; j < r.x + r.width; j++)
    {
        if(r.y >= 0 && r.y < ROWS && j >= 0 && j < COLS)
            canvas[r.y][j] = '*';

        if(r.y + r.height - 1 >= 0 &&
           r.y + r.height - 1 < ROWS &&
           j >= 0 && j < COLS)
            canvas[r.y + r.height - 1][j] = '*';
    }
    for(i = r.y; i < r.y + r.height; i++)
    {
        if(i >= 0 && i < ROWS && r.x >= 0 && r.x < COLS)
            canvas[i][r.x] = '*';
        if(i >= 0 &&
           i < ROWS &&
           r.x + r.width - 1 >= 0 &&
           r.x + r.width - 1 < COLS)
            canvas[i][r.x + r.width - 1] = '*';
    }
}

void redrawCanvas()
{
    int i;
    initializeCanvas();
    for(i = 0; i < rectCount; i++)
    {
        drawRectangle(rects[i]);
    }
}

void addRectangle()
{
    Rectangle r;

    r.id = nextId++;
    r.active = 1;
    printf("Enter x y: ");
    scanf("%d%d", &r.x, &r.y);
    printf("Enter width height: ");
    scanf("%d%d", &r.width, &r.height);
    rects[rectCount++] = r;
    redrawCanvas();
    printf("Rectangle added. ID = %d\n", r.id);
}

void deleteRectangle()
{
    int id;
    int i;
    printf("Enter rectangle ID to delete: ");
    scanf("%d", &id);
    for(i = 0; i < rectCount; i++)
    {
        if(rects[i].id == id && rects[i].active)
        {
            rects[i].active = 0;

            redrawCanvas();

            printf("Rectangle deleted.\n");
            return;
        }
    }
    printf("Rectangle not found.\n");
}

void modifyRectangle()
{
    int id;
    int i;

    printf("Enter rectangle ID to modify: ");
    scanf("%d", &id);

    for(i = 0; i < rectCount; i++)
    {
        if(rects[i].id == id && rects[i].active)
        {
            printf("Enter new x y: ");
            scanf("%d%d", &rects[i].x, &rects[i].y);
            printf("Enter new width height: ");
            scanf("%d%d",
                  &rects[i].width,
                  &rects[i].height);
            redrawCanvas();
            printf("Rectangle modified.\n");
            return;
        }
    }
    printf("Rectangle not found.\n");
}

void listRectangles()
{
    int i;
    printf("\nRectangles:\n");
    for(i = 0; i < rectCount; i++)
    {
        if(rects[i].active)
        {
            printf("ID=%d X=%d Y=%d W=%d H=%d\n",
                   rects[i].id,
                   rects[i].x,
                   rects[i].y,
                   rects[i].width,
                   rects[i].height);
        }
    }
}

int main()
{
    int choice;
    initializeCanvas();
    while(1)
    {
        printf("\n");
        printf("1. Add Rectangle\n");
        printf("2. Delete Rectangle\n");
        printf("3. Modify Rectangle\n");
        printf("4. Display Canvas\n");
        printf("5. List Rectangles\n");
        printf("6. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        switch(choice)
        {
            case 1:
                addRectangle();
                break;

            case 2:
                deleteRectangle();
                break;

            case 3:
                modifyRectangle();
                break;

            case 4:
                displayCanvas();
                break;

            case 5:
                listRectangles();
                break;

            case 6:
                return 0;

            default:
                printf("Invalid choice\n");
        }
    }
    return 0;
}