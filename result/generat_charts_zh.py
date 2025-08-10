import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import numpy as np
import os


def get_chart(title, xlabel, df):
    filtered_df = df[df[title] > -0.0001]

    if filtered_df.empty:
        return
    
    fig, ax = plt.subplots(figsize=(16, 5))

    colors = plt.cm.tab20(np.arange(len(filtered_df)))

    bars = filtered_df.set_index("库名")[title].plot(kind="barh", ax=ax, color=colors)

    for i, (value, name) in enumerate(zip(filtered_df[title], filtered_df["库名"])):
        if isinstance(value, float):
            ax.text(value, i, f" {value:.2f}", va="center", ha="left", fontsize=12)
        else:
            ax.text(value, i, f" {value}", va="center", ha="left", fontsize=12)

    ax.set_title(title, fontsize=16)
    ax.set_xlabel(xlabel, fontsize=14)
    ax.set_ylabel("库名", fontsize=14)

    plt.tight_layout()
    os.makedirs("charts_zh", exist_ok = True)
    plt.savefig(f"./charts_zh/{title}.png")
    plt.close()

if __name__ == "__main__":

    matplotlib.rcParams['font.family'] = 'SimHei'  # set font
    # read file
    df = pd.read_csv("result.csv")

    df['库名'] = df['Library']
    df = df.drop(columns=['Library'])

    df['基础可用性测试'] = (df['Validity'] > 0.5).astype(int)
    df = df.drop(columns=['Validity'])
    df['值类型支持性测试'] = (df['ValueType'] > 0.5).astype(int)
    df = df.drop(columns=['ValueType'])

    df['内存占用测试'] = df['Memory'].astype(int)
    df = df.drop(columns=['Memory'])


    df['混合琐碎数据解析耗时'] = df['DeserializeAll'].astype(int)
    df = df.drop(columns=['DeserializeAll'])

    df['浮点数解析耗时'] = df['DeserializeNum'].astype(int)
    df = df.drop(columns=['DeserializeNum'])

    df['字符串解析耗时'] = df['DeserializeStr'].astype(int)
    df = df.drop(columns=['DeserializeStr'])

    df['混合琐碎数据序列化耗时'] = df['SerializeAll'].astype(int)
    df = df.drop(columns=['SerializeAll'])

    df['浮点数数据序列化耗时'] = df['SerializeNum'].astype(int)
    df = df.drop(columns=['SerializeNum'])

    df['字符串数据序列化耗时'] = df['SerializeStr'].astype(int)
    df = df.drop(columns=['SerializeStr'])

    df['混合琐碎数据美化序列化耗时'] = df['PrettifyAll'].astype(int)
    df = df.drop(columns=['PrettifyAll'])

    df['浮点数数据美化序列化耗时'] = df['PrettifyNum'].astype(int)
    df = df.drop(columns=['PrettifyNum'])

    df['字符串数据美化序列化耗时'] = df['PrettifyStr'].astype(int)
    df = df.drop(columns=['PrettifyStr'])

    df['拷贝访问子数据耗时'] = df['GetChild'].astype(int)
    df = df.drop(columns=['GetChild'])

    df['拷贝添加子数据耗时'] = df['AddChild'].astype(int)
    df = df.drop(columns=['AddChild'])

    df['删除子数据耗时'] = df['DeleteChild'].astype(int)
    df = df.drop(columns=['DeleteChild'])

    get_chart("基础可用性测试", "能否正常解析数据(1/0)" , df)
    get_chart("值类型支持性测试", "根元素是否支持值类型(1/0)", df)
    get_chart("内存占用测试", "内存占用量(KB)", df)
    get_chart("混合琐碎数据解析耗时", "耗时(us)" ,df)
    get_chart("浮点数解析耗时", "耗时(us)", df)
    get_chart("字符串解析耗时", "耗时(us)" ,df)
    get_chart("混合琐碎数据序列化耗时", "耗时(us)",df)
    get_chart("浮点数数据序列化耗时", "耗时(us)",df)
    get_chart("字符串数据序列化耗时", "耗时(us)",df)
    get_chart("混合琐碎数据美化序列化耗时", "耗时(us)",df)
    get_chart("浮点数数据美化序列化耗时", "耗时(us)",df)
    get_chart("字符串数据美化序列化耗时", "耗时(us)",df)
    get_chart("拷贝访问子数据耗时", "耗时(us)",df)
    get_chart("拷贝添加子数据耗时", "耗时(us)",df)
    get_chart("删除子数据耗时", "耗时(us)",df)
