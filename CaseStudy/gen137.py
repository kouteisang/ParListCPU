import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.lines as mlines

# === 设置参数 ===
target_node = '137'
layer_num = 7  # 层数（从 0 到 6）

# === 读取与目标节点有关的边和节点 ===
nodeset = set()
layers = {str(i): [] for i in range(layer_num)}

for i in range(layer_num):
    with open(f"/home/cheng/fctree/CaseStudy/res/l{i}.txt", 'r') as fres:
        for line in fres:
            x, y = line.strip().split()
            if x == target_node or y == target_node:
                nodeset.add(x)
                nodeset.add(y)
                layers[str(i)].append((x, y))

print()
nodes = list(nodeset)

# === 创建图对象 ===
G = nx.Graph()
G.add_nodes_from(nodes)

# 节点位置：椭圆布局（Y 压缩）
pos = nx.circular_layout(G)
for node in pos:
    x, y = pos[node]
    pos[node] = (x, y * 0.6)

# 颜色定义
layer_colors = {
    '0': 'skyblue',
    '1': 'orange',
    '2': 'purple',
    '3': 'green',
    '4': 'red',
    '5': 'brown',
    '6': 'pink'
}

# === 绘图开始 ===
plt.figure(figsize=(10, 6))

# 绘制每层的边（带弯曲，避免重叠）
for i, (layer, edges) in enumerate(layers.items()):
    if not edges:
        continue  # 跳过没有边的层
    angle = 0.1 * (i - (layer_num - 1) / 2)  # 以中间层居中，对称偏移
    nx.draw_networkx_edges(
        G, pos,
        edgelist=edges,
        edge_color=layer_colors.get(layer, 'gray'),
        width=0.8,
        alpha=0.6,
        arrows=True,
        connectionstyle=f'arc3,rad={angle}'
    )

# 绘制节点
nx.draw_networkx_nodes(G, pos, node_size=10, node_color='black')

# 添加节点标签（更贴近点，字体更清晰）
for node, (x, y) in pos.items():
    plt.text(
        x, y + 0.01,
        s=node,
        fontsize=14,
        color='black',
        ha='center',
        va='bottom'
    )

# 构建图例：只保留实际使用的图层
legend_handles = []
for layer, edges in layers.items():
    if not edges:
        continue
    color = layer_colors.get(layer, 'gray')
    handle = mlines.Line2D([], [], color=color, linewidth=2, label=f'Layer {layer}')
    legend_handles.append(handle)

plt.legend(
    handles=legend_handles,
    title='Interaction Type',
    loc='lower center',
    bbox_to_anchor=(0.5, -0.08),
    ncol=5,
    fontsize=9,
    title_fontsize=9,
    frameon=False
)

plt.axis('off')

# 自动设置坐标范围，避免节点太远被裁剪
all_x = [x for x, y in pos.values()]
all_y = [y for x, y in pos.values()]
plt.xlim(min(all_x) - 0.1, max(all_x) + 0.1)
plt.ylim(min(all_y) - 0.1, max(all_y) + 0.1)

# 保存图像
output_file = f"new_network_plot_target_{target_node}.png"
plt.savefig(output_file, dpi=300, pad_inches=0.05)
print(f"✅ 图像已保存为 {output_file}")


total_edges = sum(len(edges) for edges in layers.values())
print(f"📊 与节点 {target_node} 有交互的边总数：{total_edges}")
