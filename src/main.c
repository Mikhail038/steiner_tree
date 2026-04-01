#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "cJSON.h"
#include <stdbool.h>
#include <time.h>

typedef struct 
{
    int id;
    int x;
    int y;
    char type;   // 't' - terminal, 's' - steiner
} point_t;

typedef struct
{
    int id;
    int u;
    int v;
} edge_t;


int manhattan(point_t a, point_t b)
{
    return abs(a.x - b.x) + abs(a.y - b.y);
}

int find_point_by_id(point_t *points, int points_n, int id)
{
    for (int i = 0; i < points_n; i++)
    {
        if (points[i].id == id)
        {
            return i;
        }
    }
    return -1;
}

double prim_mst(point_t *points, int n, int *parent)
{
    bool *visited = calloc(n, sizeof(bool)); //fill 0
    int *min_dist = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        min_dist[i] = INT_MAX;
    }

    visited[0] = true;
    for (int i = 1; i < n; i++)
    {
        min_dist[i] = manhattan(points[0], points[i]);
        parent[i] = 0;
    }

    double total = 0.0;
    for (int k = 1; k < n; k++)
    {
        int u = -1;
        int best = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && min_dist[i] < best)
            {
                best = min_dist[i];
                u = i;
            }
        }
        if (u == -1)
        {
            break;    // disconnected graph
        }
        visited[u] = true;
        total += best;
        for (int v = 0; v < n; v++)
        {
            if (!visited[v])
            {
                int d = manhattan(points[u], points[v]);
                if (d < min_dist[v])
                {
                    min_dist[v] = d;
                    parent[v] = u;
                }
            }
        }
    }

    free(visited);
    free(min_dist);
    return total;
}

double prim_mst_no_parent(point_t *points, int n)
{
    bool *visited = calloc(n, sizeof(bool)); //fill 0
    int *min_dist = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        min_dist[i] = INT_MAX;
    }

    visited[0] = true;
    for (int i = 1; i < n; i++)
    {
        min_dist[i] = manhattan(points[0], points[i]);
    }

    double total = 0.0;
    for (int k = 1; k < n; k++)
    {
        int u = -1;
        int best = INT_MAX;
        for (int i = 0; i < n; i++)
        {
            if (!visited[i] && min_dist[i] < best)
            {
                best = min_dist[i];
                u = i;
            }
        }
        if (u == -1)
        {
            break;    // disconnected graph
        }
        visited[u] = true;
        total += best;
        for (int v = 0; v < n; v++)
        {
            if (!visited[v])
            {
                int d = manhattan(points[u], points[v]);
                if (d < min_dist[v])
                {
                    min_dist[v] = d;
                }
            }
        }
    }

    free(visited);
    free(min_dist);
    return total;
}

void get_hanan_candidates(point_t *points, int n, int **cand_x, 
                int **cand_y, int *cand_count)
{
    int *x_unique = malloc(n * sizeof(int));
    int *y_unique = malloc(n * sizeof(int));
    int x_unique_n = 0, y_unique_n = 0;

    bool x_exist, y_exist;
    for (int i = 0; i < n; i++)
    {
        x_exist = false;
        for (int j = 0; j < x_unique_n; j++)
        {
            if (x_unique[j] == points[i].x)
            {
                x_exist = true;
                break;
            }
        }
        if (!x_exist)
        {
            x_unique[x_unique_n++] = points[i].x;
        }

        y_exist = false;
        for (int j = 0; j < y_unique_n; j++)
        {
            if (y_unique[j] == points[i].y)
            {
                y_exist = true;
                break;
            }
        }
        if (!y_exist)
        {
            y_unique[y_unique_n++] = points[i].y;
        }
    }

    int max_cand = x_unique_n * y_unique_n;
    int *x_candidate = malloc(max_cand * sizeof(int));
    int *y_candidate = malloc(max_cand * sizeof(int));

    int cnt = 0;
    for (int i = 0; i < x_unique_n; i++)
    {
        for (int j = 0; j < y_unique_n; j++)
        {
            int x = x_unique[i], y = y_unique[j];
            bool exists = false;
            for (int k = 0; k < n; k++)
            {
                if (points[k].x == x && points[k].y == y)
                {
                    exists = true;
                    break;
                }
            }
            if (!exists)
            {
                x_candidate[cnt] = x;
                y_candidate[cnt] = y;
                cnt++;
            }
        }
    }

    *cand_x = x_candidate;
    *cand_y = y_candidate;
    *cand_count = cnt;

    free(x_unique);
    free(y_unique);
}

void build_adj(point_t *points, int n, edge_t *edges, int m,
                int ***adj, int **adj_sizes)
{
    *adj = malloc(n * sizeof(int*));
    *adj_sizes = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++)
    {
        (*adj)[i] = malloc(n * sizeof(int));
    }

    for (int i = 0; i < m; i++)
    {
        int u_idx = find_point_by_id(points, n, edges[i].u);
        int v_idx = find_point_by_id(points, n, edges[i].v);
        if (u_idx == -1 || v_idx == -1)
        {
            continue;
        }
        (*adj)[u_idx][(*adj_sizes)[u_idx]++] = v_idx;
        (*adj)[v_idx][(*adj_sizes)[v_idx]++] = u_idx;
    }
}

void free_adj(int **adj, int n)
{
    for (int i = 0; i < n; i++)
    {
        free(adj[i]);
    }
    free(adj);
}

void basic_algorithm(point_t *terminals, int terms_n,
                point_t **result_points, int *points_n,
                edge_t **result_edges, int *edges_n)
{
    point_t *points = malloc(terms_n * sizeof(point_t));
    memcpy(points, terminals, terms_n * sizeof(point_t));
    
    int n = terms_n;
    int next_id = 0;
    for (int i = 0; i < n; i++)
    {
        if (points[i].id > next_id)
        {
            next_id = points[i].id;
        }
    }
    next_id += 1;

    while (1)
    {
        double cur_len = prim_mst_no_parent(points, n);

        int *cand_x = NULL, *cand_y = NULL, cand_count = 0;
        get_hanan_candidates(points, n, &cand_x, &cand_y, &cand_count);

        double best_gain = 0;
        int best_x = 0, best_y = 0;

        for (int i = 0; i < cand_count; i++)
        {
            point_t cand = {0, cand_x[i], cand_y[i], 's'};
            point_t *temp_points = malloc((n + 1) * sizeof(point_t));
            memcpy(temp_points, points, n * sizeof(point_t));
            temp_points[n] = cand;

            double new_len = prim_mst_no_parent(temp_points, n + 1);

            double gain = cur_len - new_len;
            if (gain > best_gain)
            {
                best_gain = gain;
                best_x = cand_x[i];
                best_y = cand_y[i];
            }

            free(temp_points);
        }

        free(cand_x);
        free(cand_y);

        if (best_gain <= 0)
        {
            break;
        }

        points = realloc(points, (n + 1) * sizeof(point_t));
        points[n].id = next_id++;
        points[n].x = best_x;
        points[n].y = best_y;
        points[n].type = 's';
        n++;
    }

    int *parent = malloc(n * sizeof(int));
    prim_mst(points, n, parent);

    edge_t *edges = malloc((n - 1) * sizeof(edge_t));
    int m = 0;
    for (int i = 1; i < n; i++)
    {
        edges[m].id = m + 1;
        edges[m].u = points[i].id;
        edges[m].v = points[parent[i]].id;
        m++;
    }
    free(parent);

    for (int i = 0; i < m; i++)
    {
        edges[i].id = i + 1;
    }

    *result_points = points;
    *points_n = n;
    *result_edges = edges;
    *edges_n = m;
}


void local_search(point_t **points_ptr, int *n_ptr,
                edge_t **edges_ptr, int *m_ptr)
{
    point_t *points = *points_ptr;
    int n = *n_ptr;
    edge_t *edges = *edges_ptr;
    int m = *m_ptr;

    int improved = 1;
    while (improved)
    {
        improved = 0;
        int *parent = malloc(n * sizeof(int));
        double cur_len = prim_mst(points, n, parent);
        free(parent);

        for (int i = 0; i < n; i++)
        {
            if (points[i].type != 's')
            {
                continue;
            }

            int ox = points[i].x, oy = points[i].y;
            double best_gain = 0;
            int best_dx = 0, best_dy = 0;

            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0)
                    {
                        continue;
                    }
                    int nx = ox + dx, ny = oy + dy;

                    int occupied = 0;
                    for (int j = 0; j < n; j++)
                    {
                        if (j != i && points[j].x == nx && points[j].y == ny)
                        {
                            occupied = 1;
                            break;
                        }
                    }
                    if (occupied)
                    {
                        continue;
                    }

                    point_t *temp_points = malloc(n * sizeof(point_t));
                    memcpy(temp_points, points, n * sizeof(point_t));
                    temp_points[i].x = nx;
                    temp_points[i].y = ny;

                    int *temp_parent = malloc(n * sizeof(int));
                    double new_len = prim_mst(temp_points, n, temp_parent);
                    free(temp_parent);

                    double gain = cur_len - new_len;
                    if (gain > best_gain)
                    {
                        best_gain = gain;
                        best_dx = dx;
                        best_dy = dy;
                    }

                    free(temp_points);
                }
            }

            if (best_gain > 0)
            {
                points[i].x += best_dx;
                points[i].y += best_dy;
                improved = 1;
                break;
            }
        }
    }

    // Перестраиваем MST после всех перемещений
    int *parent = malloc(n * sizeof(int));
    prim_mst(points, n, parent);

    edge_t *new_edges = malloc((n - 1) * sizeof(edge_t));
    int new_m = 0;
    for (int i = 1; i < n; i++)
    {
        new_edges[new_m].id = new_m + 1;
        new_edges[new_m].u = points[i].id;
        new_edges[new_m].v = points[parent[i]].id;
        new_m++;
    }
    free(parent);

    free(edges);
    *edges_ptr = new_edges;
    *m_ptr = new_m;
}

void modified_algorithm(point_t *terminals, int terms_n,
                point_t **result_points, int *points_n,
                edge_t **result_edges, int *edges_n)
{
    basic_algorithm(terminals, terms_n, result_points, points_n, result_edges, edges_n);
}

void write_output(char *filename, point_t *points, int n, edge_t *edges, int m)
{
    cJSON *root = cJSON_CreateObject();
    cJSON *node_arr = cJSON_CreateArray();
    cJSON *edge_arr = cJSON_CreateArray();

    for (int i = 0; i < n; i++)
    {
        cJSON *node = cJSON_CreateObject();
        cJSON_AddNumberToObject(node, "id", points[i].id);
        cJSON_AddNumberToObject(node, "x", points[i].x);
        cJSON_AddNumberToObject(node, "y", points[i].y);
        cJSON_AddStringToObject(node, "type", points[i].type == 't' ? "t" : "s");

        cJSON *edges_list = cJSON_CreateArray();
        for (int j = 0; j < m; j++)
        {
            if (edges[j].u == points[i].id || edges[j].v == points[i].id)
            {
                cJSON_AddItemToArray(edges_list, cJSON_CreateNumber(edges[j].id));
            }
        }
        cJSON_AddItemToObject(node, "edges", edges_list);
        cJSON_AddItemToArray(node_arr, node);
    }

    for (int j = 0; j < m; j++)
    {
        cJSON *edge = cJSON_CreateObject();
        cJSON_AddNumberToObject(edge, "id", edges[j].id);
        cJSON *verts = cJSON_CreateArray();
        cJSON_AddItemToArray(verts, cJSON_CreateNumber(edges[j].u));
        cJSON_AddItemToArray(verts, cJSON_CreateNumber(edges[j].v));
        cJSON_AddItemToObject(edge, "vertices", verts);
        cJSON_AddItemToArray(edge_arr, edge);
    }

    cJSON_AddItemToObject(root, "node", node_arr);
    cJSON_AddItemToObject(root, "edge", edge_arr);

    char *json_str = cJSON_Print(root);
    FILE *f = fopen(filename, "w");
    if (f)
    {
        fprintf(f, "%s", json_str);
        fclose(f);
    }
    free(json_str);
    cJSON_Delete(root);
}

point_t *read_input(char *filename, int *n)
{
    FILE *f = fopen(filename, "r");
    if (!f)
    {
        printf("Cannot open file %s\n", filename);
        return NULL;
    }
    fseek(f, 0, SEEK_END);
    long len = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *data = malloc(len + 1);
    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root)
    {
        printf("JSON parse error\n");
        return NULL;
    }

    cJSON *node_arr = cJSON_GetObjectItem(root, "node");
    if (!node_arr || !cJSON_IsArray(node_arr))
    {
        if (cJSON_IsArray(root))
        {
            node_arr = root;
        }
        else
        {
            printf("No 'node' array found\n");
            cJSON_Delete(root);
            return NULL;
        }
    }

    int count = cJSON_GetArraySize(node_arr);
    point_t *points = malloc(count * sizeof(point_t));
    for (int i = 0; i < count; i++)
    {
        cJSON *item = cJSON_GetArrayItem(node_arr, i);
        cJSON *id = cJSON_GetObjectItem(item, "id");
        cJSON *x = cJSON_GetObjectItem(item, "x");
        cJSON *y = cJSON_GetObjectItem(item, "y");
        if (!id || !x || !y)
        {
            printf("Missing id/x/y in node\n");
            free(points);
            cJSON_Delete(root);
            return NULL;
        }
        points[i].id = id->valueint;
        points[i].x = x->valueint;
        points[i].y = y->valueint;
        points[i].type = 't';
    }

    *n = count;
    cJSON_Delete(root);
    return points;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s [-m] <test.json>\n", argv[0]);
        return 1;
    }

    int modified = 0;
    char *filename = NULL;

    if (strcmp(argv[1], "-m") == 0)
    {
        if (argc < 3)
        {
            printf("Missing filename after -m\n");
            return 1;
        }
        modified = 1;
        filename = argv[2];
    }
    else
    {
        filename = argv[1];
    }

    int terms_n = 0;
    point_t *terminals = read_input(filename, &terms_n);
    if (!terminals)
    {
        return 1;
    }

    point_t *points = NULL;
    edge_t *edges = NULL;
    int points_n = 0;
    int edges_n = 0;

    struct timespec start, end;

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (modified)
    {
        modified_algorithm(terminals, terms_n, &points, &points_n, &edges, &edges_n);
    }
    else
    {
        basic_algorithm(terminals, terms_n, &points, &points_n, &edges, &edges_n);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    
    char outfile[256];
    char *dot = strrchr(filename, '.');
    if (dot && strcmp(dot, ".json") == 0)
    {
        int len = dot - filename;
        strncpy(outfile, filename, len);
        outfile[len] = '\0';
        strcat(outfile, "_out.json");
    }
    else
    {
        sprintf(outfile, "%s_out.json", filename);
    }
    
    write_output(outfile, points, points_n, edges, edges_n);
    
    int total_len = 0;
    for (int i = 0; i < edges_n; i++)
    {
        int ui = find_point_by_id(points, points_n, edges[i].u);
        int vi = find_point_by_id(points, points_n, edges[i].v);
        total_len += manhattan(points[ui], points[vi]);
    }
    
    printf("time [%6.3f ms] length [%3d]\n", elapsed * 1000, total_len);
    // printf("Output written to %s\n", outfile);
    
    free(terminals);
    free(points);
    free(edges);

    return 0;
}