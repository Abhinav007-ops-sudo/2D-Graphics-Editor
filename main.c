#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#define ROWS 30
#define COLS 80
#define MAX  100
//shapes
#define RECT 1
#define LINE 2
#define CIRC 3
#define TRI  4

typedef struct {
    int id, type, active;
    int col1, row1;   
    int col2, row2;   
    int col3, row3;  
    int radius;       
    int width;        
    int height;      
} Shape;

char  canvas[ROWS][COLS];
Shape shapes[MAX];
int   count=0, next_id=1;

//Cnavas

void canvas_init(void) {
    int r,c;
    for (r=0; r<ROWS;r++)
        for (c=0;c<COLS;c++)
            canvas[r][c]='.';
}

void pixel(int row, int col, char ch) {
    if (row>=0&&row<ROWS && col>=0 && col<COLS)
        canvas[row][col] = ch;
}

void canvas_show(void) {
    int r, c;
    /* Column ruler */
    printf("\n     ");
    for (c=0; c<COLS; c+=10)
        printf("%-10d",c);
    printf("\n     ");
    for (c=0; c<COLS;c++)
        printf("%c", (c%10==0) ? '|' : '-');
    printf("\n");

    for (r = 0; r < ROWS; r++) {
        printf("%3d |", r);
        for (c = 0; c < COLS; c++)
            printf("%c", canvas[r][c]);
        printf("\n");
    }
    printf("     ");
    for (c = 0; c < COLS; c++)
        printf("%c", (c % 10 == 0) ? '|' : '-');
    printf("\n");
}

void draw_line(int c1, int r1, int c2, int r2, char ch) {
    int dc = abs(c2 - c1), dr = abs(r2 - r1);
    int sc = (c1 < c2) ? 1 : -1, sr = (r1 < r2) ? 1 : -1;
    int err = dc - dr, e2;
    for (;;) {
        pixel(r1, c1, ch);
        if (c1 == c2 && r1 == r2) break;
        e2 = 2 * err;
        if (e2 > -dr) { err -= dr; c1 += sc; }
        if (e2 <  dc) { err += dc; r1 += sr; }
    }
}

void draw_circle(int cc, int cr, int rad, char ch) {
    int x = 0, y = rad, d = 1 - rad;
    while (x <= y) {
        pixel(cr + y, cc + x, ch); pixel(cr + y, cc - x, ch);
        pixel(cr - y, cc + x, ch); pixel(cr - y, cc - x, ch);
        pixel(cr + x, cc + y, ch); pixel(cr + x, cc - y, ch);
        pixel(cr - x, cc + y, ch); pixel(cr - x, cc - y, ch);
        if (d < 0) d += 2 * x + 3;
        else     { d += 2 * (x - y) + 5; y--; }
        x++;
    }
}

void draw_rect(int left, int top, int w, int h, char ch) {
    int r, c;
    for (c = left; c < left + w; c++) {
        pixel(top,         c, ch);
        pixel(top + h - 1, c, ch);
    }
    for (r = top; r < top + h; r++) {
        pixel(r, left,         ch);
        pixel(r, left + w - 1, ch);
    }
}

void draw_triangle(int c1, int r1, int c2, int r2,
                   int c3, int r3, char ch) {
    draw_line(c1, r1, c2, r2, ch);
    draw_line(c2, r2, c3, r3, ch);
    draw_line(c3, r3, c1, r1, ch);
}

//Redraw

void shape_draw(Shape *s) {
    if (!s->active) return;
    switch (s->type) {
        case RECT:
            draw_rect(s->col1, s->row1, s->width, s->height, '*');
            break;
        case LINE:
            draw_line(s->col1, s->row1, s->col2, s->row2, '#');
            break;
        case CIRC:
            draw_circle(s->col1, s->row1, s->radius, 'O');
            break;
        case TRI:
            draw_triangle(s->col1, s->row1,
                          s->col2, s->row2,
                          s->col3, s->row3, '@');
            break;
    }
}

void redraw(void) {
    int i;
    canvas_init();
    for (i = 0; i < count; i++)
        shape_draw(&shapes[i]);
}

const char *type_name(int t) {
    switch (t) {
        case RECT: return "Rectangle";
        case LINE: return "Line";
        case CIRC: return "Circle";
        case TRI:  return "Triangle";
        default:   return "Unknown";
    }
}

int read_int_clamped(const char *prompt, int lo, int hi) {
    int v;
    for (;;) {
        printf("%s [%d–%d]: ", prompt, lo, hi);
        if (scanf("%d", &v) != 1) { while (getchar() != '\n'); continue; }
        if (v < lo || v > hi) {
            printf("  ! Must be between %d and %d.\n", lo, hi);
        } else {
            return v;
        }
    }
}

//Add shape
void add_rect(void) {
    Shape s;
    memset(&s, 0, sizeof(s));
    s.id = next_id++; s.type = RECT; s.active = 1;

    printf("\n-- Add Rectangle --\n");
    printf("Top-left corner:\n");
    s.col1  = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row1  = read_int_clamped("  row    (y)", 0, ROWS - 1);
    printf("Size:\n");
    s.width  = read_int_clamped("  width  (cols)", 1, COLS - s.col1);
    s.height = read_int_clamped("  height (rows)", 1, ROWS - s.row1);

    shapes[count++] = s;
    redraw();
    printf("Rectangle added (id=%d).\n", s.id);
}

void add_line(void) {
    Shape s;
    memset(&s, 0, sizeof(s));
    s.id = next_id++; s.type = LINE; s.active = 1;

    printf("\n-- Add Line --\n");
    printf("Start point:\n");
    s.col1 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);
    printf("End point:\n");
    s.col2 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row2 = read_int_clamped("  row    (y)", 0, ROWS - 1);

    shapes[count++] = s;
    redraw();
    printf("Line added (id=%d).\n", s.id);
}

void add_circle(void) {
    Shape s;
    int max_r;
    memset(&s, 0, sizeof(s));
    s.id = next_id++; s.type = CIRC; s.active = 1;

    printf("\n-- Add Circle --\n");
    printf("Centre:\n");
    s.col1 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);

    /* Max radius that keeps circle on canvas */
    max_r = s.col1;
    if (COLS - 1 - s.col1 < max_r) max_r = COLS - 1 - s.col1;
    if (s.row1            < max_r) max_r = s.row1;
    if (ROWS - 1 - s.row1 < max_r) max_r = ROWS - 1 - s.row1;
    if (max_r < 1) max_r = 1;   /* allow even if partial */

    s.radius = read_int_clamped("  radius", 1, max_r);

    shapes[count++] = s;
    redraw();
    printf("Circle added (id=%d).\n", s.id);
}

void add_triangle(void) {
    Shape s;
    memset(&s, 0, sizeof(s));
    s.id = next_id++; s.type = TRI; s.active = 1;

    printf("\n-- Add Triangle --\n");
    printf("Vertex 1:\n");
    s.col1 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);
    printf("Vertex 2:\n");
    s.col2 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row2 = read_int_clamped("  row    (y)", 0, ROWS - 1);
    printf("Vertex 3:\n");
    s.col3 = read_int_clamped("  column (x)", 0, COLS - 1);
    s.row3 = read_int_clamped("  row    (y)", 0, ROWS - 1);

    shapes[count++] = s;
    redraw();
    printf("Triangle added (id=%d).\n", s.id);
}

//Delete and Modify

void delete_shape(void) {
    int id, i;
    printf("Enter shape id to delete: ");
    scanf("%d", &id);
    for (i = 0; i < count; i++) {
        if (shapes[i].id == id && shapes[i].active) {
            shapes[i].active = 0;
            redraw();
            printf("Deleted id=%d.\n", id);
            return;
        }
    }
    printf("No active shape with id=%d.\n", id);
}

void modify_shape(void) {
    int id, i;
    printf("Enter shape id to modify: ");
    scanf("%d", &id);
    for (i = 0; i < count; i++) {
        if (shapes[i].id != id || !shapes[i].active) continue;
        Shape *s = &shapes[i];
        printf("\n-- Modify %s (id=%d) --\n", type_name(s->type), id);
        switch (s->type) {
            case RECT:
                printf("New top-left corner:\n");
                s->col1   = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row1   = read_int_clamped("  row    (y)", 0, ROWS - 1);
                printf("New size:\n");
                s->width  = read_int_clamped("  width  (cols)", 1, COLS - s->col1);
                s->height = read_int_clamped("  height (rows)", 1, ROWS - s->row1);
                break;
            case LINE:
                printf("New start point:\n");
                s->col1 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                printf("New end point:\n");
                s->col2 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row2 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                break;
            case CIRC: {
                int max_r;
                printf("New centre:\n");
                s->col1 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                max_r = s->col1;
                if (COLS - 1 - s->col1 < max_r) max_r = COLS - 1 - s->col1;
                if (s->row1             < max_r) max_r = s->row1;
                if (ROWS - 1 - s->row1  < max_r) max_r = ROWS - 1 - s->row1;
                if (max_r < 1) max_r = 1;
                s->radius = read_int_clamped("  radius", 1, max_r);
                break;
            }
            case TRI:
                printf("New vertex 1:\n");
                s->col1 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row1 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                printf("New vertex 2:\n");
                s->col2 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row2 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                printf("New vertex 3:\n");
                s->col3 = read_int_clamped("  column (x)", 0, COLS - 1);
                s->row3 = read_int_clamped("  row    (y)", 0, ROWS - 1);
                break;
        }
        redraw();
        printf("Shape id=%d updated.\n", id);
        return;
    }
    printf("No active shape with id=%d.\n", id);
}

void list_shapes(void) {
    int i, found = 0;
    printf("\n%-4s  %-10s  %s\n", "id", "type", "details");
    printf("----  ----------  -----------------------------------\n");
    for (i = 0; i < count; i++) {
        Shape *s = &shapes[i];
        if (!s->active) continue;
        found = 1;
        printf("%-4d  %-10s  ", s->id, type_name(s->type));
        switch (s->type) {
            case RECT:
                printf("top-left=(%d,%d)  size=%dx%d",
                       s->col1, s->row1, s->width, s->height);
                break;
            case LINE:
                printf("(%d,%d) → (%d,%d)",
                       s->col1, s->row1, s->col2, s->row2);
                break;
            case CIRC:
                printf("centre=(%d,%d)  radius=%d",
                       s->col1, s->row1, s->radius);
                break;
            case TRI:
                printf("(%d,%d)  (%d,%d)  (%d,%d)",
                       s->col1, s->row1,
                       s->col2, s->row2,
                       s->col3, s->row3);
                break;
        }
        printf("\n");
    }
    if (!found) printf("No shapes yet.\n");
}

//Menus
void add_menu(void) {
    int c;
    if (count >= MAX) { printf("Shape limit reached.\n"); return; }
    printf("\nWhat shape?\n"
           "  1. Rectangle (*)\n"
           "  2. Line      (#)\n"
           "  3. Circle    (O)\n"
           "  4. Triangle  (@)\n"
           "choice: ");
    scanf("%d", &c);
    switch (c) {
        case 1: add_rect();     break;
        case 2: add_line();     break;
        case 3: add_circle();   break;
        case 4: add_triangle(); break;
        default: printf("Invalid choice.\n");
    }
}

int main(void) {
    int c;
    canvas_init();

    printf("=== ASCII Canvas (%d cols x %d rows) ===\n", COLS, ROWS);
    printf("Symbols:  Rectangle=*  Line=#  Circle=O  Triangle=@\n");
    printf("Canvas:   column (x): 0=left … %d=right\n", COLS - 1);
    printf("          row    (y): 0=top  … %d=bottom\n", ROWS - 1);

    for (;;) {
        printf("\n1.Add shape  2.Delete  3.Modify  "
               "4.Show canvas  5.List shapes  6.Exit\nchoice: ");
        if (scanf("%d", &c) != 1) { while (getchar() != '\n'); continue; }
        switch (c) {
            case 1: add_menu();     break;
            case 2: delete_shape(); break;
            case 3: modify_shape(); break;
            case 4: canvas_show();  break;
            case 5: list_shapes();  break;
            case 6: return 0;
            default: printf("Invalid choice.\n");
        }
    }
}