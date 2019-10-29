import pandas as pd
import plotly.graph_objects as go

df = pd.read_csv('../output.csv')

data = go.Scatter(x = df['0'], y = df['1'], name='Accuracy', line = go.scatter.Line(color="#0077a7"))

fig = go.Figure(data)

fig.update_layout(title='Accuracy to # of Iteration of Discrete HMM.', plot_bgcolor='rgb(230, 230,230)', showlegend=True)
fig.update_xaxes(title_text='# of iteration', range=[0, 300])
fig.update_yaxes(title_text='Accuracy', range=[0.4, 0.9])

fig.write_image("../plot.svg")
fig.show()
