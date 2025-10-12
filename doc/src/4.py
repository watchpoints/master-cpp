import os

JOB_DIR = "./chapter4"
OUTPUT_FILE = "./chapter4/README.md"

def generate_markdown_links():
    files = [f for f in os.listdir(JOB_DIR) if f.endswith(".md")]
    files.sort()  # 可改为自定义排序逻辑（如提取前缀数字）

    lines = []
    for f in files:
        display_name = f[:-3]  # 去掉 .md
        lines.append(f"- [{display_name}](./chapter4/{f})")
    return "\n".join(lines)

def main():
    content = generate_markdown_links()
    with open(OUTPUT_FILE, "w", encoding="utf-8") as f:
        f.write(" " +content)
    print(f"✅ 已生成 {OUTPUT_FILE}，共 {len(os.listdir(JOB_DIR))} 条记录。")

if __name__ == "__main__":
    main()

