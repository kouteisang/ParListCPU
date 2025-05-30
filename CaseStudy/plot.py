import networkx as nx
import matplotlib.pyplot as plt
import matplotlib.lines as mlines

# 读取节点列表
nodes = []
with open("/home/cheng/fctree/CaseStudy/homo_k_1_lmd_5_pathlmd.txt", 'r') as fg:
    for idx, line in enumerate(fg):
        if idx == 0:
            continue
        nodes.append(line.strip())


# 读取多层边
layers = {str(i): [] for i in range(7)}
for i in range(7):
    with open(f"/home/cheng/fctree/CaseStudy/res/l{i}.txt", 'r') as fres:
        for line in fres:
            x, y = line.strip().split()
            layers[str(i)].append((x, y))

# 创建图并添加节点
G = nx.Graph()
G.add_nodes_from(nodes)

# 每层的颜色（可自定义）
layer_colors = {
    '0': 'skyblue',
    '1': 'orange',
    '2': 'purple',
    '3': 'green',
    '4': 'red',
    '5': 'brown',
    '6': 'pink'
}

# ✅ 圆形布局 + 椭圆变换
pos = nx.circular_layout(G)
for node in pos:
    x, y = pos[node]
    pos[node] = (x, y * 0.6)  # 压缩 Y 坐标形成椭圆

# ✅ 图像尺寸调整为椭圆适配
plt.figure(figsize=(10, 6))

# ✅ 绘制多层边
for layer, edges in layers.items():
    nx.draw_networkx_edges(
        G, pos,
        edgelist=edges,
        edge_color=layer_colors.get(layer, 'gray'),
        width=0.5,
        alpha=0.6
    )

# ✅ 绘制节点（小黑点）
nx.draw_networkx_nodes(G, pos, node_size=10, node_color='black')

for node, (x, y) in pos.items():
    plt.text(
        x, y + 0.035,     # 向上微移
        s=node,
        fontsize=5,
        ha='center',
        va='center'
    )

# ✅ 手动构建图例，避免重复
legend_handles = []
for layer in sorted(layers.keys()):
    color = layer_colors.get(layer, 'gray')
    handle = mlines.Line2D([], [], color=color, linewidth=2, label=f'Layer {layer}')
    legend_handles.append(handle)

plt.legend(
    handles=legend_handles,
    title='Interaction Type',
    loc='lower center',
    bbox_to_anchor=(0.5, -0.08),
    ncol=4,
    fontsize=7,
    title_fontsize=8,
    frameon=False
)

plt.axis('off')

# 添加这个裁剪 + 保存段
all_x = [x for x, y in pos.values()]
all_y = [y for x, y in pos.values()]
plt.xlim(min(all_x) - 0.05, max(all_x) + 0.05)
plt.ylim(min(all_y) - 0.05, max(all_y) + 0.05)


# ✅ 保存图像为高清 PNG
plt.savefig("network_plot_ellipse_labeled.png", dpi=300, bbox_inches='tight', pad_inches=0.05)
print("✅ 图像已保存为 network_plot_ellipse_labeled.png")
