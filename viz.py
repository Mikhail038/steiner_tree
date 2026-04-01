import json
import sys
import matplotlib.pyplot as plt
import matplotlib.patches as mpatches

def manhattan(p1, p2):
    return abs(p1[0] - p2[0]) + abs(p1[1] - p2[1])

def read_json(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return data

def plot_graph(data, title):
    nodes = data.get('node', [])
    edges = data.get('edge', [])

    # Словари для быстрого доступа
    node_dict = {}
    terminals_x = []
    terminals_y = []
    terminals_id = []
    steiner_x = []
    steiner_y = []
    steiner_id = []

    for node in nodes:
        nid = node['id']
        x = node['x']
        y = node['y']
        typ = node.get('type', 't')  # по умолчанию терминал
        node_dict[nid] = (x, y)
        if typ == 't':
            terminals_x.append(x)
            terminals_y.append(y)
            terminals_id.append(nid)
        else:
            steiner_x.append(x)
            steiner_y.append(y)
            steiner_id.append(nid)

    # Вычисляем общую длину дерева (если есть рёбра)
    total_len = 0
    for edge in edges:
        u, v = edge['vertices']
        total_len += manhattan(node_dict[u], node_dict[v])

    plt.figure(figsize=(8, 6))

    # Рисуем терминалы
    plt.scatter(terminals_x, terminals_y, c='red', s=100, label='Terminals', zorder=5)
    for i, txt in enumerate(terminals_id):
        plt.annotate(txt, (terminals_x[i], terminals_y[i]), xytext=(5,5),
                     textcoords='offset points', fontsize=9, color='darkred')

    # Рисуем точки Штейнера
    if steiner_x:
        plt.scatter(steiner_x, steiner_y, c='green', s=100, marker='s', label='Steiner points', zorder=5)
        for i, txt in enumerate(steiner_id):
            plt.annotate(txt, (steiner_x[i], steiner_y[i]), xytext=(5,5),
                         textcoords='offset points', fontsize=9, color='darkgreen')

    # Рисуем рёбра
    for edge in edges:
        u, v = edge['vertices']
        x_coords = [node_dict[u][0], node_dict[v][0]]
        y_coords = [node_dict[u][1], node_dict[v][1]]
        plt.plot(x_coords, y_coords, 'b-', linewidth=1, alpha=0.7)

    # Оформление
    plt.title(f"{title}\nTotal length: {total_len}")
    plt.xlabel("X")
    plt.ylabel("Y")
    plt.grid(True, linestyle='--', alpha=0.5)
    plt.axis('equal')  # равный масштаб по осям

    # Легенда
    red_patch = mpatches.Patch(color='red', label='Terminal')
    green_patch = mpatches.Patch(color='green', label='Steiner point')
    plt.legend(handles=[red_patch, green_patch])

    plt.tight_layout()
    plt.show()

def main():
    if len(sys.argv) < 2:
        print("Usage: python visualize.py <filename.json>")
        sys.exit(1)

    filename = sys.argv[1]
    try:
        data = read_json(filename)
    except Exception as e:
        print(f"Error reading file: {e}")
        sys.exit(1)

    plot_graph(data, filename)

if __name__ == "__main__":
    main()