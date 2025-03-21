import pandas as pd
import matplotlib.pyplot as plt
import matplotlib
import os


def get_chart(title, xlabel, df):

    filtered_df = df[df[title] > -0.5]
    # 创建一个图形和轴
    fig, ax = plt.subplots(figsize=(16, 5))

    # 绘制水平条形图
    filtered_df.set_index("库名")[title].plot(kind="barh", ax=ax)

    # 设置标题和标签
    ax.set_title(title, fontsize=16)
    ax.set_xlabel(xlabel, fontsize=14)
    ax.set_ylabel("库名", fontsize=14)

    # 保存图表为图片
    plt.tight_layout()
    os.makedirs("images_zh", exist_ok = True)
    plt.savefig(f"./images_zh/{title}.png")
    # plt.show()

if __name__ == "__main__":

    matplotlib.rcParams['font.family'] = 'SimHei'  # 指定字体为黑体
    # 读取CSV文件
    df = pd.read_csv("result.csv")

    df['库名'] = df['lib_name']
    df = df.drop(columns=['lib_name'])

    df['正常解析测试'] = (df['validity'] > 0.5).astype(int)
    df = df.drop(columns=['validity'])
    df['数值类型支持'] = (df['value_type'] > 0.5).astype(int)
    df = df.drop(columns=['value_type'])

    df['反序列化测试'] = (df['unserialize_1'] + 0.1 * df['unserialize_2'] + 2.*df['unserialize_3'])/1000.
    df = df.drop(columns=['unserialize_1' , 'unserialize_2', 'unserialize_3'])

    df['序列化测试'] = (df['serialize_1'] + 0.1 * df['serialize_2'] + 2.*df['serialize_3'])/1000.
    df = df.drop(columns=['serialize_1' , 'serialize_2', 'serialize_3'])

    df['美化序列化测试'] = (df['prettify_1'] + 0.1 * df['prettify_2'] + 2.*df['prettify_3'])/1000.
    df = df.drop(columns=['prettify_1' , 'prettify_2', 'prettify_3'])

    df['获取子元素测试'] = df['get_child'] / 1000.

    df['增加子元素测试'] = df['add_child'] / 1000.

    df['删除子元素测试'] = df['delete_child'] / 1000.


    df['增删改查加权测试'] = df['get_child'] * 0.6 + df['add_child']*0.25 + df['delete_child']*0.15
    df = df.drop(columns=['get_child' , 'add_child', 'delete_child'])

    df['数值格式支持测试'] = df['double_validity']
    df = df.drop(columns=['double_validity'])

    df['浮点数精度测试'] = df['double_accuracy']
    df = df.drop(columns=['double_accuracy'])

    df['字符串转义测试'] = df['string_validity']
    df = df.drop(columns=['string_validity'])

    df['语法严格性测试'] = df['error_validity']
    df = df.drop(columns=['error_validity'])


    get_chart("正常解析测试", "能否正常解析(1/0)" , df)
    get_chart("数值类型支持", "根支持为数值类型(1/0)", df)
    get_chart("反序列化测试", "加权耗时(毫秒)" ,df)
    get_chart("序列化测试", "加权耗时(毫秒)", df)
    get_chart("美化序列化测试", "加权耗时(毫秒)" ,df)
    get_chart("获取子元素测试", "累计耗时(毫秒)",df)
    get_chart("增加子元素测试", "累计耗时(毫秒)",df)
    get_chart("删除子元素测试", "累计耗时(毫秒)",df)
    get_chart("增删改查加权测试", "加权耗时(毫秒)",df)
    get_chart("数值格式支持测试", "分数(100满)",df)
    get_chart("浮点数精度测试", "分数(100满)",df)
    get_chart("字符串转义测试", "分数(100满)",df)
    get_chart("语法严格性测试", "分数(100满)",df)

