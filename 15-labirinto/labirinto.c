#include <stdio.h>

#define MAX_W 75
#define MAX_H 75

struct node{
    int color; // 0 = blue. 1 = violet
    int was_visited; // checks if the node was tried in a cycle checking

    int top_left; // -1 = doesn't have. 0 = blocked. 1 = available
    int bottom_left;
    int top_right;
    int bottom_right;
};

void startNode(struct node * new_node, int color){ // Initializes the properties of a node
    new_node->color = color;
    new_node->was_visited = 0;

    new_node->top_left = -1;
    new_node->top_right = -1;
    new_node->bottom_left = -1;
    new_node->bottom_right = -1;
}
struct node graph[MAX_H * 2 - 1][MAX_W]; // Global node matrix

void nodeStatus(int last_x, int last_y, int x, int y, int * next_x, int * next_y, int * continuable_cycle, int * endable_cycle){
    if(graph[y][x].was_visited){ // Node already visited AND it was the last visited = Invalid path
        if (x != last_x || y != last_y) *endable_cycle = 1; // Node already visited = The cycle may end here
        return;
    }
    *continuable_cycle = 1; // Node wasn't visited = The cycle checking may continue
    *next_x = x;
    *next_y = y;
    return;
}

int searchCycle(int x, int y, int last_x, int last_y, int current_cycle){
    if(graph[y][x].was_visited) return 0; // Ignores visited nodes
    
    int continuable_cycle = 0;
    int next_x, next_y;
    int endable_cycle = 0;

    int temp_y = y - 1; // temp_x, temp_y: Next node's coordinates
    if(graph[y][x].top_left == 1){
        int temp_x = (graph[y][x].color == 0) ? x : x - 1;
        nodeStatus(last_x, last_y, temp_x, temp_y, &next_x, &next_y, &continuable_cycle, &endable_cycle);
    }
    if(graph[y][x].top_right == 1){
        int temp_x = (graph[y][x].color == 0) ? x + 1: x;
        nodeStatus(last_x, last_y, temp_x, temp_y, &next_x, &next_y, &continuable_cycle, &endable_cycle);
    }
    temp_y = y + 1;
    if(graph[y][x].bottom_left == 1){
        int temp_x = (graph[y][x].color == 0) ? x : x - 1;
        nodeStatus(last_x, last_y, temp_x, temp_y, &next_x, &next_y, &continuable_cycle, &endable_cycle);
    }
    if(graph[y][x].bottom_right == 1){
        int temp_x = (graph[y][x].color == 0) ? x + 1 : x;
        nodeStatus(last_x, last_y, temp_x, temp_y, &next_x, &next_y, &continuable_cycle, &endable_cycle);
    }

    // printf("node: [%d, %d]", y, x);
    // printf("| ENDABLE: %d | CONTINUABLE: %d | CYCLE: %d\n", endable_cycle, continuable_cycle, current_cycle);
    // if(continuable_cycle)
    //     printf("NEXT: [%d, %d]\n", next_x, next_x);
    // putchar('\n');

    graph[y][x].was_visited = 1; // Marks the node as visited
    if(continuable_cycle)
        return searchCycle(next_x, next_y, x, y, current_cycle + 1);
    if(endable_cycle)
        return current_cycle;
    return 0;
}

int main(){
    int w, h;
    scanf("%d %d\n", &w, &h); // Maze Length

    char maze[MAX_H][MAX_W + 1];
    for(int i = 1; w != 0 || h != 0; i++, scanf("%d %d\n", &w, &h)){ // Maze Input
        for(int j=0; j < h; j++)
            scanf("%s", maze[j]);
        

        for(int j = 0; j < w - 1; j++) // First Blue Line
            startNode(&graph[0][j], 0);

        for(int j = 1; j < h * 2 - 1; j++){
            for(int k = 0; k < w; k++){ // Violet Line
                startNode(&graph[j][k], 1);

                if(maze[(j - 1) / 2][k] == '/'){
                    graph[j][k].top_left = 0;
                    if(k > 0) graph[j - 1][k - 1].bottom_right = 0;
                    graph[j][k].top_right = 1;
                    graph[j - 1][k].bottom_left = 1;
                }
                else{
                    graph[j][k].top_left = 1;
                    if(k > 0) graph[j - 1][k - 1].bottom_right = 1;
                    graph[j][k].top_right = 0;
                    graph[j - 1][k].bottom_left = 0;
                }
            }
            graph[j][0].top_left = -1;
            graph[j][w - 1].top_right = -1;
            // graph[j - 1][-1].bottom_right = -1;
            graph[j - 1][w - 1].bottom_left = -1;
            j++;

            for(int k = 0; k < w - 1; k++){ // Blue Line
                startNode(&graph[j][k], 0);

                if(maze[j / 2][k] == '/'){
                    graph[j][k].top_left = 0;
                    graph[j - 1][k].bottom_right = 0;
                }
                else{
                    graph[j][k].top_left = 1;
                    graph[j - 1][k].bottom_right = 1;
                }

                if(maze[j / 2][k + 1] == '/'){
                    graph[j][k].top_right = 1;
                    graph[j - 1][k + 1].bottom_left = 1;
                }
                else{
                    graph[j][k].top_right = 0;
                    graph[j - 1][k + 1].bottom_left = 0;
                }
            }
        }

        // for(int j = 0; j < h * 2 - 1; j++){ // Prints the node matrix
        //     for(int k = 0; k < w - 1; k++){
        //         printf("%d [%d %d %d %d] ", graph[j][k].color, graph[j][k].top_left, graph[j][k].top_right, graph[j][k].bottom_left, graph[j][k].bottom_right);
        //     }
        //     if(j % 2 != 0) printf("%d [%d %d %d %d] ", graph[j][w - 1].color, graph[j][w - 1].top_left, graph[j][w - 1].top_right, graph[j][w - 1].bottom_left, graph[j][w - 1].bottom_right);
        //     putchar('\n');
        // }
        
        int cycles = 0;
        int longest_cycle = 0;

        for(int j = 0; j < h * 2 - 1; j++){
            for(int k = 0; k < w - 1; k++){
                if(!graph[j][k].was_visited){ // Checks if the node can create a cycle
                    int cycle_report = searchCycle(k, j, -1, -1, 1);
                    cycles = (cycle_report) ? cycles + 1 : cycles;
                    longest_cycle = (cycle_report > longest_cycle) ? cycle_report : longest_cycle;
                }
            }
        }

        if(i > 1) putchar('\n');
        printf("Maze #%d:\n", i); // Output
        if(cycles)
            printf("%d Cycles; the longest has length %d.\n", cycles, longest_cycle);
        else
            puts("There are no cycles.");

        scanf("%d %d\n", &w, &h); // Input
    }
    return 0;
}