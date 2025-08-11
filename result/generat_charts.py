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

    bars = filtered_df.set_index("lib_name")[title].plot(kind="barh", ax=ax, color=colors)

    for i, (value, name) in enumerate(zip(filtered_df[title], filtered_df["lib_name"])):
        if isinstance(value, float):
            ax.text(value, i, f" {value:.2f}", va="center", ha="left", fontsize=12)
        else:
            ax.text(value, i, f" {value}", va="center", ha="left", fontsize=12)

    ax.set_title(title, fontsize=16)
    ax.set_xlabel(xlabel, fontsize=14)
    ax.set_ylabel("lib_name", fontsize=14)

    plt.tight_layout()
    os.makedirs("charts", exist_ok = True)
    plt.savefig(f"./charts/{title}.png")
    plt.close()

if __name__ == "__main__":

    matplotlib.rcParams['font.family'] = 'SimHei'  # set font
    # read file
    df = pd.read_csv("result.csv")

    df['lib_name'] = df['Library']
    df = df.drop(columns=['Library'])

    df['Basic_validity_Test'] = (df['Validity'] > 0.5).astype(int)
    df = df.drop(columns=['Validity'])
    df['Value_type_support_Test'] = (df['ValueType'] > 0.5).astype(int)
    df = df.drop(columns=['ValueType'])

    df['Memory_usage_Test'] = df['Memory'].astype(int)
    df = df.drop(columns=['Memory'])


    df['Deserialize_mixed_speed_Test'] = df['DeserializeAll'].astype(int)
    df = df.drop(columns=['DeserializeAll'])

    df['Deserialize_number_speed_Test'] = df['DeserializeNum'].astype(int)
    df = df.drop(columns=['DeserializeNum'])

    df['Deserialize_string_speed_Test'] = df['DeserializeStr'].astype(int)
    df = df.drop(columns=['DeserializeStr'])

    df['Serialize_mixed_speed_Test'] = df['SerializeAll'].astype(int)
    df = df.drop(columns=['SerializeAll'])

    df['Serialize_number_speed_Test'] = df['SerializeNum'].astype(int)
    df = df.drop(columns=['SerializeNum'])

    df['Serialize_string_speed_Test'] = df['SerializeStr'].astype(int)
    df = df.drop(columns=['SerializeStr'])

    df['Pretty_serialize_mixed_speed_Test'] = df['PrettifyAll'].astype(int)
    df = df.drop(columns=['PrettifyAll'])

    df['Pretty_serialize_number_speed_Test'] = df['PrettifyNum'].astype(int)
    df = df.drop(columns=['PrettifyNum'])

    df['Pretty_serialize_string_speed_Test'] = df['PrettifyStr'].astype(int)
    df = df.drop(columns=['PrettifyStr'])

    df['Get_value_and_copy_speed_Test'] = df['GetChild'].astype(int)
    df = df.drop(columns=['GetChild'])

    df['Add_child_and_copy_speed_Test'] = df['AddChild'].astype(int)
    df = df.drop(columns=['AddChild'])

    df['Delete_child_speed_Test'] = df['DeleteChild'].astype(int)
    df = df.drop(columns=['DeleteChild'])

    get_chart("Basic_validity_Test", "Can it be resolved normally(1/0)" , df)
    get_chart("Value_type_support_Test", "Does the root support value types(1/0)", df)
    get_chart("Memory_usage_Test", "Memory usage after deserialization(KB)", df)
    get_chart("Deserialize_mixed_speed_Test", "Desialization time - mixed data(us)" ,df)
    get_chart("Deserialize_number_speed_Test", "Desialization time - many float number(us)", df)
    get_chart("Deserialize_string_speed_Test", "Desialization time - many string(us)" ,df)
    get_chart("Serialize_mixed_speed_Test", "Serialization time - mixed data(us)",df)
    get_chart("Serialize_number_speed_Test", "Serialization time - many float number(us)",df)
    get_chart("Serialize_string_speed_Test", "Serialization time - many string(us)",df)
    get_chart("Pretty_serialize_mixed_speed_Test", "Pretty serialization time - mixed data(us)",df)
    get_chart("Pretty_serialize_number_speed_Test", "Pretty serialization time - many float number(us)",df)
    get_chart("Pretty_serialize_string_speed_Test", "Pretty serialization time - many string(us)",df)
    get_chart("Get_value_and_copy_speed_Test", "Time for copying and accessing sub elements(us)",df)
    get_chart("Add_child_and_copy_speed_Test", "Time for copying and adding sub elements(us)",df)
    get_chart("Delete_child_speed_Test", "Time for deleting sub elements(us)",df)
