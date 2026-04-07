import json
import random

def generate_test(num_points, filename, x_range=(0, 200), y_range=(0, 200)):
    nodes = []
    for i in range(1, num_points + 1):
        node = {
            "name": f"p{i}",
            "x": random.randint(x_range[0], x_range[1]),
            "y": random.randint(y_range[0], y_range[1]),
            "id": i,
            "type": "t"
        }
        nodes.append(node)
    data = {"node": nodes, "edge": []}
    with open(filename, "w") as f:
        json.dump(data, f, indent=4)
    print(f"Generated {filename} with {num_points} points")

if __name__ == "__main__":
    random.seed(42)
    generate_test(31, "dat/0031_0000.json", (0, 200), (0, 200))
    generate_test(32, "dat/0032_0000.json", (0, 200), (0, 200))
    generate_test(33, "dat/0033_0000.json", (0, 200), (0, 200))
    generate_test(34, "dat/0034_0000.json", (0, 200), (0, 200))
    generate_test(35, "dat/0035_0000.json", (0, 200), (0, 200))
    generate_test(36, "dat/0036_0000.json", (0, 200), (0, 200))
    generate_test(37, "dat/0037_0000.json", (0, 200), (0, 200))
    generate_test(38, "dat/0038_0000.json", (0, 200), (0, 200))
    generate_test(39, "dat/0039_0000.json", (0, 200), (0, 200))
    generate_test(40, "dat/0040_0000.json", (0, 200), (0, 200))

