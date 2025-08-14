import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
from collections import Counter

# === 参数设置 ===
target_node = '137'
layer_num = 7

# === Step 1: 读取路径节点（只标注这些点）
highlight_nodes = []
with open("/home/cheng/fctree/CaseStudy/homo_k_1_lmd_5_pathlmd.txt", 'r') as fg:
    for idx, line in enumerate(fg):
        if idx == 0:
            continue
        highlight_nodes.append(line.strip())
highlight_nodes_set = set(highlight_nodes)

# === Step 2: 读取所有边，同时记录高亮边（保留重复层）
layers = {str(i): [] for i in range(layer_num)}
highlight_edges = []  # ((u,v), layer)
highlight_edge_set = set()

for i in range(layer_num):
    with open(f"/home/cheng/fctree/CaseStudy/res/l{i}.txt", 'r') as fres:
        for line in fres:
            x, y = line.strip().split()
            edge = (x, y)
            layers[str(i)].append(edge)

            if x == target_node or y == target_node:
                highlight_edges.append(((x, y), str(i)))  # 多层保留
                highlight_edge_set.add((x, y))

# ✅ 统计重复边
cnt = Counter(edge for edge, _ in highlight_edges)
repeated = [e for e, v in cnt.items() if v > 1]
print(f"📌 有 {len(repeated)} 条边在多个层中出现")
print(f"📊 与节点 {target_node} 有交互的边总数（含重复层数）：{len(highlight_edges)}")

# === Step 3: 创建图并添加所有边
G = nx.MultiGraph()  # 支持多条边
for edges in layers.values():
    G.add_edges_from(edges)

# === Step 4: 布局（椭圆）
pos = nx.circular_layout(G)
for node in pos:
    x, y = pos[node]
    pos[node] = (x, y * 0.6)

# === Step 5: 定义颜色
layer_colors = {
    '0': '#E41A1C',  # 强红
    '1': '#377EB8',  # 高饱蓝
    '2': '#4DAF4A',  # 高饱绿
    '3': '#984EA3',  # 深紫
    '4': '#FF7F00',  # 橙
    '5': '#A65628',  # 棕褐
    '6': '#F781BF',  # 饱和粉
}

# === Step 6: 绘图
plt.figure(figsize=(10, 6))

# 1️⃣ 所有边灰色背景（带弯曲）
for i, (layer, edges) in enumerate(layers.items()):
    angle = 0.1 * (i - (layer_num - 1) / 2)
    nx.draw_networkx_edges(
        G, pos,
        edgelist=edges,
        edge_color='lightgray',
        width=0.5,
        alpha=0.3,
        connectionstyle=f'arc3,rad={angle}'
    )

# 2️⃣ 与 target_node 有交互的边高亮（按层角度绘制）
for (edge, layer) in highlight_edges:
    angle = 0.05 * (int(layer) - (layer_num - 1) / 2)
    nx.draw_networkx_edges(
        G, pos,
        edgelist=[edge],
        edge_color=layer_colors.get(layer, 'black'),
        width=0.9,
        alpha=1.0,
        connectionstyle=f'arc3,rad={angle}'
    )

# 3️⃣ 所有节点黑点
nx.draw_networkx_nodes(G, pos, node_size=10, node_color='black')

# 4️⃣ 路径节点 label（137 为红）
for node in highlight_nodes:
    if node in pos:
        x, y = pos[node]
        label_color = 'red' if node == target_node else 'black'
        plt.text(
            x, y + 0.02,
            s=node,
            color=label_color,
            fontsize=9,
            ha='center',
            va='center'
        )

# 5️⃣ 图例（只包含用到的层）
used_layers = sorted(set(layer for _, layer in highlight_edges), key=int)
legend_handles = [
    mlines.Line2D([], [], color=layer_colors.get(layer, 'gray'), linewidth=2, label=f'Layer {layer}')
    for layer in used_layers
]

plt.legend(
    handles=legend_handles,
    title='Interaction Type',
    loc='lower center',
    bbox_to_anchor=(0.5, -0.08),
    ncol=min(7, len(legend_handles)),
    fontsize=10,
    title_fontsize=11,
    frameon=False
)

# 6️⃣ 其他设置
plt.axis('off')
all_x = [x for x, y in pos.values()]
all_y = [y for x, y in pos.values()]
plt.xlim(min(all_x) - 0.05, max(all_x) + 0.05)
plt.ylim(min(all_y) - 0.05, max(all_y) + 0.05)

# === 保存图像 ===
output_file = f"hcnetwork_plot_highlight_{target_node}.png"
plt.savefig(output_file, dpi=300, bbox_inches='tight', pad_inches=0.05)
print(f"✅ 图像已保存为 {output_file}")
